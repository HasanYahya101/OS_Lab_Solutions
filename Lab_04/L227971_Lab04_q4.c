#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <stdbool.h>
#include <string.h>

int gcd(int a, int b)
{
    while (b != 0)
    {
        int temp = b;
        b = a % b;
        a = temp;
    }
    return a;
}

bool is_prime(int num)
{
    if (num <= 1)
        return false;
    for (int i = 2; i * i <= num; i++)
    {
        if (num % i == 0)
            return false;
    }
    return true;
}

int main()
{
    int pipe1[2], pipe2[2];
    if (pipe(pipe1) == -1 || pipe(pipe2) == -1)
    {
        printf("Error: Pipe creation failed");
        return 1;
    }

    int pid1 = fork();
    if (pid1 < 0)
    {
        printf("Error: Fork failed");
        return 1;
    }

    if (pid1 > 0)
    {
        close(pipe1[0]);
        close(pipe2[1]);

        int num_elements;
        printf("Enter the number of elements: ");
        scanf("%d", &num_elements);

        while (num_elements <= 0)
        {
            printf("Error: Number of elements should be greater than 0\n");
            printf("Enter the number of elements: ");
            scanf("%d", &num_elements);
        }

        int arr[num_elements];
        printf("Enter the elements: ");
        for (int i = 0; i < num_elements; i++)
        {
            scanf("%d", &arr[i]);
        }

        write(pipe1[1], &num_elements, sizeof(int));
        write(pipe1[1], arr, sizeof(arr));
        close(pipe1[1]);

        char result_msg[50];
        int bytes_read = read(pipe2[0], result_msg, sizeof(result_msg));
        close(pipe2[0]);

        result_msg[bytes_read] = '\0';
        printf("%s\n", result_msg);
        wait(NULL);
    }
    else
    {
        int pid2 = fork();
        if (pid2 < 0)
        {
            printf("Error: Fork failed");
            return 1;
        }

        if (pid2 > 0) // Child process 1: Bob
        {
            close(pipe1[1]);
            close(pipe2[0]);

            dup2(pipe1[0], 0);
            close(pipe1[0]);
            dup2(pipe2[1], 1);
            close(pipe2[1]);

            int num_elements;
            read(0, &num_elements, sizeof(int));
            int arr[num_elements];
            read(0, arr, sizeof(arr));

            int result_gcd = arr[0];
            for (int i = 1; i < num_elements; i++)
            {
                result_gcd = gcd(result_gcd, arr[i]);
            }

            // Send the GCD to Charlie via stdout
            write(1, &result_gcd, sizeof(int));

            exit(0);
        }
        else // Child process 2: Charlie
        {
            close(pipe1[0]);
            close(pipe1[1]);
            close(pipe2[1]);

            dup2(pipe2[0], 0);
            close(pipe2[0]);

            // Read the GCD from Bob via stdin
            int result_gcd;
            read(0, &result_gcd, sizeof(int));

            // Check if the GCD is prime and send the result to Alice
            char message[50];
            if (is_prime(result_gcd))
            {
                sprintf(message, "The GCD is prime");
            }
            else
            {
                sprintf(message, "The GCD is not prime");
            }

            write(1, message, strlen(message) + 1); // Send the actual message length

            exit(0);
        }
    }

    return 0;
}
