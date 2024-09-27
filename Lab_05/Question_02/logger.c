#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <time.h>

void generate_system_status(char *status, size_t len)
{
    time_t t = time(NULL);
    struct tm *tm_info = localtime(&t);
    char timestamp[64];
    strftime(timestamp, sizeof(timestamp), "%Y-%m-%d %H:%M:%S", tm_info);

    int random_val = rand() % 100;
    snprintf(status, len, "[%s] System status: %d%% load\n", timestamp, random_val);
}

int main()
{
    srand(time(NULL));

    int fifo_fd;
    char buffer[256];

    fifo_fd = open("/tmp/log_fifo", O_WRONLY);
    if (fifo_fd == -1)
    {
        printf("Error opening FIFO");
        exit(1);
    }

    while (1)
    {
        generate_system_status(buffer, 256);
        write(fifo_fd, buffer, strlen(buffer));

        sleep(5);
    }

    close(fifo_fd);
    return 0;
}
