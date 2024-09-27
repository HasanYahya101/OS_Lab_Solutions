#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>

int main()
{
    char expression[256];
    char result[256];
    int server_fd, client_fd;

    mkfifo("/tmp/client_fifo", 0666);

    while (1)
    {
        printf("Enter a mathematical expression (or 'exit' to quit): ");
        fgets(expression, 256, stdin);
        expression[strcspn(expression, "\n")] = 0; // Remove newline character

        if (strcmp(expression, "exit") == 0)
        {
            break;
        }
        server_fd = open("/tmp/server_fifo", O_WRONLY);
        write(server_fd, expression, strlen(expression) + 1);
        close(server_fd);

        client_fd = open("/tmp/client_fifo", O_RDONLY);
        read(client_fd, result, 256); // Read result from server
        close(client_fd);

        printf("Result: %s\n", result);
    }

    unlink("/tmp/client_fifo");
    return 0;
}
