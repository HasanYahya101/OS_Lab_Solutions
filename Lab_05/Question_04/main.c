#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/select.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <time.h>

int main()
{
    // Create FIFOs for each sensor
    mkfifo("/tmp/sensor1_fifo", 0666);
    mkfifo("/tmp/sensor2_fifo", 0666);
    mkfifo("/tmp/sensor3_fifo", 0666);

    // Fork to create child processes for each sensor
    if (fork() == 0)
    {
        execlp("./sensor1", "sensor1", NULL);
        perror("Failed to execute sensor1");
        exit(1);
    }

    if (fork() == 0)
    {
        execlp("./sensor2", "sensor2", NULL);
        perror("Failed to execute sensor2");
        exit(1);
    }

    if (fork() == 0)
    {
        execlp("./sensor3", "sensor3", NULL);
        perror("Failed to execute sensor3");
        exit(1);
    }

    // Open FIFOs for reading
    int fd1 = open("/tmp/sensor1_fifo", O_RDONLY | O_NONBLOCK);
    int fd2 = open("/tmp/sensor2_fifo", O_RDONLY | O_NONBLOCK);
    int fd3 = open("/tmp/sensor3_fifo", O_RDONLY | O_NONBLOCK);

    while (1)
    {
        srand(time(NULL));
        int temp1, temp2, temp3;
        int bytes_read;

        // Read from sensor 1 FIFO
        bytes_read = read(fd1, &temp1, sizeof(temp1) - 1);
        if (bytes_read > 0)
        {
            printf("Sensor 1: %d\n", temp1);
        }

        // Read from sensor 2 FIFO
        bytes_read = read(fd2, &temp2, sizeof(temp2) - 1);
        if (bytes_read > 0)
        {
            printf("Sensor 2: %d\n", temp2);
        }

        // Read from sensor 3 FIFO
        bytes_read = read(fd3, &temp3, sizeof(temp3) - 1);
        if (bytes_read > 0)
        {
            printf("Sensor 3: %d\n", temp3);
        }

        // Sleep for a short period to avoid busy-waiting
        usleep(100000); // 100 milliseconds
    }

    // Close FIFOs and unlink
    close(fd1);
    close(fd2);
    close(fd3);
    unlink("/tmp/sensor1_fifo");
    unlink("/tmp/sensor2_fifo");
    unlink("/tmp/sensor3_fifo");
    return 0;
}
