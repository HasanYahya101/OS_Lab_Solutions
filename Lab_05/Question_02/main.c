#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/stat.h>

int main()
{
    mkfifo("/tmp/log_fifo", 0666);

    pid_t logger_pid, reader_pid;
    logger_pid = fork();
    if (logger_pid < 0)
    {
        printf("Fork failed for logger");
        exit(1);
    }
    else if (logger_pid == 0)
    {
        execlp("./logger", "logger", NULL);
        printf("Failed to exec logger");
        exit(1);
    }
    reader_pid = fork();
    if (reader_pid < 0)
    {
        printf("Fork failed for reader");
        exit(1);
    }
    else if (reader_pid == 0)
    {
        execlp("./reader", "reader", NULL);
        printf("Failed to exec reader");
        exit(1);
    }
    wait(NULL);
    wait(NULL);

    return 0;
}
