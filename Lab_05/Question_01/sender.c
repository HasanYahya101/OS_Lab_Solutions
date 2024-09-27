#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <unistd.h>

int main()
{
    int fd;
    char buffer[256];
    mkfifo("/tmp/fifo_pipe", 0666);

    printf("Sender ready. Enter your message:\n");

    while (1)
    {
        fd = open("/tmp/fifo_pipe", O_WRONLY);
        if (fd == -1)
        {
            printf("Failed to open FIFO for writing");
            exit(1);
        }
        fgets(buffer, 256, stdin);
        write(fd, buffer, strlen(buffer) + 1);
        close(fd);
    }

    return 0;
}
