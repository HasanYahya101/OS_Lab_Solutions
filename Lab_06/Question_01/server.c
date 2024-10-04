#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <fcntl.h>
#include <stdlib.h>
#include <string.h>

int main()
{
    int shmid;
    char *memory;

    shmid = shmget(100, 1024, 0666); // 100 is key and 1024 is size

    if (shmid == -1)
    {
        printf("Shared memory creation failed\n");
        return 1;
    }

    char buffer[100];

    memory = shmat(shmid, NULL, 0);
    if (memory == (char *)-1)
    {
        printf("Shared memory attachment failed\n");
        return 1;
    }

    // add the whole line to buffer from mem
    strcpy(buffer, memory);

    int sum = 0;
    int count = 0;
    char *token = strtok(buffer, " ");
    while (token != NULL)
    {
        sum += atoi(token);
        count++;
        token = strtok(NULL, " ");
    }

    printf("Sum: %d\n", sum);
    printf("Average: %f\n", (float)sum / count);
    wait(NULL);
    shmdt(memory);

    return 0;
}