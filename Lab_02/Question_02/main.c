#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/wait.h>

int main(int argc, char *argv[])
{
    if (argc != 3)
    {
        printf("Error, Please enter valid arguments");
        exit(1);
    }

    int pid = fork();

    if (pid < 0)
    {
        printf("Error, Fork failed");
        exit(1);
    }

    if (pid == 0)
    {
        int inputFile = open(argv[1], 0);  // read
        int outputFile = open(argv[2], 1); // write

        if (inputFile < 0 || outputFile < 0)
        {
            printf("Error: Couldn't oopen file");
            exit(1);
        }

        char char_data[1024];
        int bytesRead;
        while ((bytesRead = read(inputFile, char_data, 1026)) > 0)
        {
            for (int i = 0; i < bytesRead; i++)
            {
                if (isdigit(char_data[i]))
                {
                    if (write(outputFile, &char_data[i], 1) < 0)
                    {
                        printf("Error: File not open");
                        close(inputFile);
                        close(outputFile);
                        exit(1);
                    }
                }
            }
        }

        close(inputFile);
        close(outputFile);
        exit(0);
    }
    else
    {
        wait(NULL);

        int outputFile = open(argv[2], O_RDONLY);
        if (outputFile < 0)
        {
            printf("Error opening file");
            exit(1);
        }

        int sum = 0, count = 0;
        char chars[1024];
        int bytesRead;
        while ((bytesRead = read(outputFile, chars, 1024)) > 0)
        {
            for (int i = 0; i < bytesRead; i++)
            {
                if (isdigit(chars[i]))
                {
                    sum += chars[i] - '0';
                    count++;
                }
            }
        }

        close(outputFile);

        if (count > 0)
        {
            int average = sum / count;
            printf("Sum: %d\n", sum);
            printf("Average: %d\n", average);
        }
        else
        {
            printf("No numbers found in the output file.\n");
        }
    }

    return 0;
}