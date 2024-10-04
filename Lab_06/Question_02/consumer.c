#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/ipc.h>
#include <sys/shm.h>

int main()
{
    int shmid;  // for buffer
    int shmid2; // for bool flag
    char *memory;

    shmid = shmget(102, 1024, IPC_CREAT | 0666); // 102 is key and 1024 is size
    shmid2 = shmget(101, 1024, IPC_CREAT | 0666);

    if (shmid == -1 || shmid2 == -1)
    {
        printf("Shared memory creation failed\n");
        return 1;
    }

    // check if flag is 1 (true)
    int *flag = shmat(shmid2, NULL, 0);
    if (*flag == 0)
    {
        printf("No Data already in shared memory. The producer needs to add one before you can read it.\n");
        return 1;
    }

    // attach to shared memory
    memory = shmat(shmid, NULL, 0);

    printf("Data in shared memory: %s\n", memory);

    // put the flag as false in shared memory
    *flag = 0;

    shmdt(memory);
    shmdt(flag);
    return 0;
}