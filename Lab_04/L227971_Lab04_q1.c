#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <ctype.h>
#include <string.h>
#include <sys/wait.h>

void reverse_case(char *str)
{
    for (int i = 0; str[i] != '\0'; i++)
    {
        if (islower(str[i]))
        {
            str[i] = toupper(str[i]);
        }
        else if (isupper(str[i]))
        {
            str[i] = tolower(str[i]);
        }
    }
}

int main()
{
    int pipefd[2];

    if (pipe(pipefd) == -1)
    {
        printf("Error: Pipe creation failed\n");
        exit(1);
    }
    int pid;
    pid = fork();
    if (pid == -1)
    {
        printf("Error: Fork failed\n");
        exit(1);
    }

    if (pid > 0)
    {
        char message[100];
        printf("Enter a message: ");
        fgets(message, sizeof(message), stdin);
        int len = strlen(message);
        if (len > 0 && message[len - 1] == '\n')
        {
            message[len - 1] = '\0';
        }
        close(pipefd[0]);
        write(pipefd[1], &len, sizeof(len));
        write(pipefd[1], message, strlen(message) + 1);
        close(pipefd[1]);
        wait(NULL);
    }
    else
    {
        int size = 0;
        close(pipefd[1]);
        read(pipefd[0], &size, sizeof(size));
        char *str = (char *)malloc(size);
        read(pipefd[0], str, size);
        close(pipefd[0]);
        reverse_case(str);
        printf("Reversed case message: %s\n", str);
        free(str);
        exit(0);
    }

    return 0;
}