#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

int main(int argc, char *argv[])
{
    if (argc != 2)
    {
        printf("Please enter number between 1 and 10 (using command)\n");
        return 1;
    }

    int n = atoi(argv[1]);

    if (n < 1 || n > 10)
    {
        printf("Please enter a number between 1 and 10.\n");
        return 1;
    }

    for (int i = 0; i < n; i++)
    {
        pid_t pid = fork();

        if (pid == 0)
        {
            printf("Child id: %d\n", getpid());
            printf("\n");

            if (i == n - 1)
            {
                return 0;
            }
        }
        else
        {
            printf("Parent id: %d\n", getpid());
            wait(NULL);
            return 0;
        }
    }

    return 0;
}
