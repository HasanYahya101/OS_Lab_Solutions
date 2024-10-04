#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/ipc.h>
#include <sys/shm.h>

int main(int argc, char *argv[])
{
    if (argc < 2)
    {
        printf("Please enter data to put in the shared memory\n");
        return 1;
    }

    int shmid;  // for buffer
    int shmid2; // for bool flag
    char *memory;

    shmid = shmget(102, 1024, IPC_CREAT | 0666);  // 102 is key and 1024 is size
    shmid2 = shmget(101, 1024, IPC_CREAT | 0666); // 101 is key and 1024 is size

    if (shmid == -1 || shmid2 == -1)
    {
        printf("Shared memory creation failed\n");
        return 1;
    }

    // check if flag is 1 (true)
    int *flag = shmat(shmid2, NULL, 0);
    if (*flag == 1)
    {
        printf("Data already present in shared memory. The consumer needs to read the data before adding new one.\n");
        return 1;
    }

    char buffer[100];
    strcpy(buffer, argv[1]);

    // attach to shared memory
    memory = shmat(shmid, NULL, 0);

    strcpy(memory, buffer);

    // put the flag as true in shared memory
    *flag = 1;

    shmdt(memory);
    shmdt(flag);

    printf("Data added to shared memory\n");
    return 0;
}