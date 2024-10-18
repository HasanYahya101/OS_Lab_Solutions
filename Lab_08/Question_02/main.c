#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>

int main(int argc, char *argv[])
{
    if (argc < 4)
    {
        printf("Error: Please enter rows, cols, and the matrix data as cmd args\n");
        return 1;
    }

    int totalElements = atoi(argv[1]) * atoi(argv[2]);

    if (argc != totalElements + 3)
    {
        printf("Error: Please enter the correct number of elements\n");
        return 1;
    }

    int rows = atoi(argv[1]);
    int cols = atoi(argv[2]);
    int matrix[rows][cols];
    int index = 3;

    for (int i = 0; i < rows; i++)
    {
        for (int j = 0; j < cols; j++)
        {
            matrix[i][j] = atoi(argv[index++]);
        }
    }

    int pipe1[2], pipe2[2], pipe3[2];
    pipe(pipe1);
    pipe(pipe2);
    pipe(pipe3);

    if (fork() == 0)
    {
        close(pipe1[0]);
        int rowSums[rows];
        for (int i = 0; i < rows; i++)
        {
            rowSums[i] = 0;
            for (int j = 0; j < cols; j++)
            {
                rowSums[i] += matrix[i][j];
            }
        }
        write(pipe1[1], rowSums, sizeof(rowSums));
        close(pipe1[1]);
        exit(0);
    }
    else
    {
        wait(NULL);
    }

    if (fork() == 0)
    {
        close(pipe2[0]);
        int colSums[cols];
        for (int j = 0; j < cols; j++)
        {
            colSums[j] = 0;
            for (int i = 0; i < rows; i++)
            {
                colSums[j] += matrix[i][j];
            }
        }
        write(pipe2[1], colSums, sizeof(colSums));
        close(pipe2[1]);
        exit(0);
    }
    else
    {
        wait(NULL);
    }

    if (fork() == 0)
    {
        close(pipe3[0]);
        int overallSum = 0;
        for (int i = 0; i < rows; i++)
        {
            for (int j = 0; j < cols; j++)
            {
                overallSum += matrix[i][j];
            }
        }
        write(pipe3[1], &overallSum, sizeof(overallSum));
        close(pipe3[1]);
        exit(0);
    }
    else
    {
        wait(NULL);
    }

    close(pipe1[1]);
    close(pipe2[1]);
    close(pipe3[1]);

    int rowSums[rows];
    int colSums[cols];
    int overallSum;

    read(pipe1[0], rowSums, sizeof(rowSums));
    read(pipe2[0], colSums, sizeof(colSums));
    read(pipe3[0], &overallSum, sizeof(overallSum));

    close(pipe1[0]);
    close(pipe2[0]);
    close(pipe3[0]);

    printf("Row sums: ");
    for (int i = 0; i < rows; i++)
    {
        printf("%d ", rowSums[i]);
    }
    printf("\n");

    printf("Column sums: ");
    for (int j = 0; j < cols; j++)
    {
        printf("%d ", colSums[j]);
    }
    printf("\n");

    printf("Overall sum: %d\n", overallSum);

    return 0;
}