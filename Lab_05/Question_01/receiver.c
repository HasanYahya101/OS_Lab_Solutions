#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <unistd.h>

int main()
{
    int fd;
    char buffer[256];
    mkfifo("/tmp/fifo_pipe", 0666);
    printf("Receiver waiting for messages...\n");

    while (1)
    {
        fd = open("/tmp/fifo_pipe", O_RDONLY);
        if (fd == -1)
        {
            printf("Failed to open FIFO for reading");
            exit(1);
        }

        if (read(fd, buffer, 256) > 0)
        {
            printf("Received: %s", buffer);
        }

        close(fd);
        sleep(1);
    }

    return 0;
}
