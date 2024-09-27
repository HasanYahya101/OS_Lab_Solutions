#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <time.h>
#include <sys/stat.h>

int main()
{
    mkfifo("/tmp/sensor3_fifo", 0666);

    srand(time(NULL));

    while (1)
    {
        srand(time(NULL));
        int temperature = rand() % 11 + 20; // Random temperature between 20 and 30
        int fifo_fd = open("/tmp/sensor3_fifo", O_WRONLY);

        write(fifo_fd, &temperature, sizeof(temperature));
        close(fifo_fd);
        sleep(3);
    }

    unlink("/tmp/sensor3_fifo");
    return 0;
}
