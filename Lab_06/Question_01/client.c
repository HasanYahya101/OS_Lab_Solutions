#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/ipc.h>
#include <sys/shm.h>

int main(int argc, char *argv[])
{
    if (argc != 2)
    {
        printf("Please pass in the name of the file\n");
        return 1;
    }

    int shmid;
    char *memory;

    shmid = shmget(100, 1024, IPC_CREAT | 0666); // 100 is key and 1024 is size
    if (shmid == -1)
    {
        printf("Shared memory creation failed\n");
        return 1;
    }

    int fd = open(argv[1], O_RDONLY);
    if (fd == -1)
    {
        printf("File not found\n");
        return 1;
    }
    char buffer[100];

    // attach to shared memory
    memory = shmat(shmid, NULL, 0);

    read(fd, buffer, 100);
    strcpy(memory, buffer);

    close(fd);
    shmdt(memory);
    return 0;
}