#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <string.h>

void calculate_sum(int fd)
{
    int pid = fork();
    if (pid == 0)
    {
        int sum = 0, num;
        char buffer[1024];
        lseek(fd, 0, 0);
        int n = read(fd, buffer, sizeof(buffer));
        buffer[n] = '\0';
        char *token = strtok(buffer, " \n");
        while (token != NULL)
        {
            num = atoi(token);
            sum += num;
            token = strtok(NULL, " \n");
        }
        printf("Sum: %d\n", sum);
        exit(0);
    }
    else
    {
        wait(NULL);
    }
}

void calculate_average(int fd)
{
    int pid = fork();
    if (pid == 0)
    {
        int sum = 0, num, count = 0;
        char buffer[1024];
        lseek(fd, 0, 0);
        int n = read(fd, buffer, sizeof(buffer));
        buffer[n] = '\0';
        char *token = strtok(buffer, " \n");
        while (token != NULL)
        {
            num = atoi(token);
            sum += num;
            count++;
            token = strtok(NULL, " \n");
        }
        if (count > 0)
        {
            float avg = (float)sum / count;
            printf("Average: %.2f\n", avg);
        }
        exit(0);
    }
    else
    {
        wait(NULL);
    }
}

void find_min_max(int fd)
{
    int pid = fork();
    if (pid == 0)
    {
        int num, min, max, first = 1;
        char data[1024];
        lseek(fd, 0, 0);
        int n = read(fd, data, sizeof(data));
        data[n] = '\0';
        char *token = strtok(data, " \n");
        while (token != NULL)
        {
            num = atoi(token);
            if (first)
            {
                min = max = num;
                first = 0;
            }
            else
            {
                if (num < min)
                    min = num;
                if (num > max)
                    max = num;
            }
            token = strtok(NULL, " \n");
        }

        printf("Min: %d\n", min);
        printf("Max: %d\n", max);
        exit(0);
    }
    else
    {
        wait(NULL);
    }
}

int main(int argc, char *argv[])
{
    if (argc != 2)
    {
        printf("Please enter a filename in command arguments\n");
        exit(1);
    }

    int fd = open(argv[1], O_RDONLY); // Open file for reading
    if (fd < 0)
    {
        printf("Error: File not open\n");
        exit(1);
    }

    calculate_sum(fd);
    calculate_average(fd);
    find_min_max(fd);

    close(fd);
    return 0;
}
