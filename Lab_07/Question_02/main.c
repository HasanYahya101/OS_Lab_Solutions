#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <string.h>
#include <sys/wait.h>

int main(int argc, char *argv[])
{
    if (argc != 3)
    {
        printf("Error, enter 3 cmd args\n");
        return 1;
    }

    int threshold = atoi(argv[2]);
    mkfifo("fifo1", 0666);
    mkfifo("fifo2", 0666);
    mkfifo("fifo3", 0666);

    int pid1, pid2, pid3;
    int fd1, fd2, fd3;

    if ((pid1 = fork()) == 0)
    {
        FILE *file = fopen(argv[1], "r");

        fd1 = open("fifo1", O_WRONLY);

        int num;
        while (fscanf(file, "%d", &num) != EOF)
        {
            int squared = num * num;
            write(fd1, &squared, sizeof(squared));
        }
        close(fd1);
        fclose(file);
        exit(0);
    }
    else
    {
        wait(NULL);
    }

    if ((pid2 = fork()) == 0)
    {
        fd1 = open("fifo1", O_RDONLY);
        fd2 = open("fifo2", O_WRONLY);
        int num, count = 0;
        double sum = 0, average;
        while (read(fd1, &num, sizeof(num)) > 0)
        {
            sum += num;
            count++;
            average = sum / count;
            write(fd2, &average, sizeof(average));
        }
        close(fd1);
        close(fd2);
        exit(0);
    }
    else
    {
        wait(NULL);
    }

    if ((pid3 = fork()) == 0)
    {
        fd2 = open("fifo2", O_RDONLY);

        fd3 = open("fifo3", O_WRONLY);

        double average;
        int count = 0;
        while (read(fd2, &average, sizeof(average)) > 0)
        {
            if (average > threshold)
            {
                count++;
            }
        }
        write(fd3, &count, sizeof(count));
        close(fd2);
        close(fd3);
        exit(0);
    }
    else
    {
        wait(NULL);
    }

    fd3 = open("fifo3", O_RDONLY);
    int finalCount;
    read(fd3, &finalCount, sizeof(finalCount));
    printf("%d running averages are greater than %d.\n", finalCount, threshold);
    close(fd3);

    unlink("fifo1");
    unlink("fifo2");
    unlink("fifo3");
    return 0;
}