#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

int main()
{
    mkfifo("/tmp/server_fifo", 0666);
    mkfifo("/tmp/client_fifo", 0666);

    pid_t server_pid, client_pid;
    server_pid = fork();
    if (server_pid < 0)
    {
        printf("Fork failed for server");
        exit(1);
    }
    else if (server_pid == 0)
    {
        // Child process runs the server
        execlp("./server", "server", NULL);
        printf("Failed to exec server");
        exit(1);
    }

    client_pid = fork();
    if (client_pid < 0)
    {
        printf("Fork failed for client");
        exit(1);
    }
    else if (client_pid == 0)
    {
        execlp("./client", "client", NULL);
        printf("Failed to exec client");
        exit(1);
    }
    wait(NULL);
    wait(NULL);

    // Cleanup
    unlink("/tmp/server_fifo");
    unlink("/tmp/client_fifo");

    return 0;
}
