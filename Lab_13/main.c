#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/wait.h>

void handle_sigchld(int sig)
{
    int status;
    wait(&status);
    printf("Received SIGCHLD\n");
    printf("Total Time in Seconds: %d\n", WEXITSTATUS(status));
}

int main()
{
    int laps, lap_time;

    printf("Enter Number of Laps: ");
    scanf("%d", &laps);
    while (laps < 1)
    {
        printf("Number of Laps must be greater than 0\n");
        printf("Enter Number of Laps: ");
        scanf("%d", &laps);
    }
    printf("Enter Lap Time: ");
    scanf("%d", &lap_time);

    while (lap_time < 1)
    {
        printf("Lap Time must be greater than 0\n");
        printf("Enter Lap Time: ");
        scanf("%d", &lap_time);
    }

    signal(SIGCHLD, handle_sigchld);

    pid_t pid = fork();
    if (pid == -1)
    {
        printf("Failed to fork\n");
        return 1;
    }
    else if (pid == 0)
    {
        for (int i = 1; i <= laps; i++)
        {
            sleep(lap_time);
            printf("Lap: %d Completed\n", i);
        }
        exit(laps * lap_time);
    }
    else
    { // Parent process
        // Parent waits for the signal (handled by the signal handler)
        pause();
    }

    return 0;
}
