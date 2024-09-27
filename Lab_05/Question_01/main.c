#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

int main()
{
    pid_t pid1, pid2;
    pid1 = fork();

    if (pid1 < 0)
    {
        printf("Fork for sender failed");
        exit(1);
    }

    if (pid1 == 0)
    {
        execlp("./sender", "sender", NULL);
        exit(0);
    }
    pid2 = fork();

    if (pid2 < 0)
    {
        printf("Fork for receiver failed");
        exit(1);
    }

    if (pid2 == 0)
    {
        execlp("./receiver", "receiver", NULL);
        exit(0);
    }

    wait(NULL);
    wait(NULL);

    printf("Both sender and receiver have completed.\n");

    return 0;
}
