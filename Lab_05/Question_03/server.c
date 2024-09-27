#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <ctype.h>

double evaluate_expression(const char *expression)
{
    double result = 0.0;
    double num1, num2;
    char operator;
    sscanf(expression, "%lf %c %lf", &num1, &operator, & num2);

    if (operator== '+')
    {
        result = num1 + num2;
    }
    else if (operator== '-')
    {
        result = num1 - num2;
    }
    else if (operator== '*')
    {
        result = num1 * num2;
    }
    else if (operator== '/')
    {
        if (num2 != 0)
        {
            result = num1 / num2;
        }
        else
        {
            printf("Can't divide by zero!\n");
        }
    }
    else
    {
        printf("Invalid operator!\n");
    }

    return result;
}

int main()
{
    char expression[256];
    char result[256];
    int server_fd, client_fd;
    mkfifo("/tmp/server_fifo", 0666);

    while (1)
    {
        server_fd = open("/tmp/server_fifo", O_RDONLY);
        read(server_fd, expression, 256);
        close(server_fd);

        if (strcmp(expression, "exit") == 0)
        {
            break;
        }
        double eval_result = evaluate_expression(expression);
        snprintf(result, 256, "%lf", eval_result);

        client_fd = open("/tmp/client_fifo", O_WRONLY);
        write(client_fd, result, strlen(result) + 1);
        close(client_fd);
    }

    unlink("/tmp/server_fifo");
    return 0;
}
