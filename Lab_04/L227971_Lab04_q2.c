#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>
#include <stdlib.h>

int main()
{
    int pipe1[2], pipe2[2];

    if (pipe(pipe1) == -1 || pipe(pipe2) == -1)
    {
        printf("Error: Pipe creation failed\n");
        return 1;
    }

    int pid;
    pid = fork();

    if (pid < 0)
    {
        printf("Error: Fork failed\n");
        return 1;
    }

    if (pid > 0)
    {
        int sum = 0;
        int num_elements;
        printf("Enter the number of elements: ");
        scanf("%d", &num_elements);

        int arr[num_elements];
        printf("Enter the elements of the array:\n");
        for (int i = 0; i < num_elements; i++)
        {
            scanf("%d", &arr[i]);
        }
        int size = sizeof(int) * num_elements;
        close(pipe1[0]);
        close(pipe2[1]);

        write(pipe1[1], &num_elements, sizeof(int));
        write(pipe1[1], arr, size);
        close(pipe1[1]);

        read(pipe2[0], &sum, sizeof(int));
        close(pipe2[0]);

        wait(NULL);

        printf("Sum of array: %d\n", sum);
    }
    else
    {
        int sum = 0;
        int num;
        close(pipe1[1]);
        close(pipe2[0]);

        read(pipe1[0], &num, sizeof(int));

        int arr[num];
        int size = sizeof(int) * num;

        read(pipe1[0], arr, size);
        close(pipe1[0]);

        for (int i = 0; i < num; i++)
        {
            sum += arr[i];
        }

        write(pipe2[1], &sum, sizeof(int));
        close(pipe2[1]);

        exit(0);
    }

    return 0;
}