#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>

int main()
{
    int fifo_fd;
    char buffer[256];

    fifo_fd = open("/tmp/log_fifo", O_RDONLY);
    if (fifo_fd == -1)
    {
        printf("Error opening FIFO");
        exit(1);
    }

    FILE *log_file = fopen("log.txt", "a");
    if (log_file == NULL)
    {
        printf("Error opening log file");
        close(fifo_fd);
        exit(1);
    }

    while (1)
    {
        int num_bytes = read(fifo_fd, buffer, 256 - 1);
        if (num_bytes > 0)
        {
            buffer[num_bytes] = '\0';
            fprintf(log_file, "%s", buffer);
            fflush(log_file);
        }
        else if (num_bytes == -1)
        {
            printf("Error reading from FIFO");
        }
    }

    fclose(log_file);
    close(fifo_fd);
    return 0;
}
