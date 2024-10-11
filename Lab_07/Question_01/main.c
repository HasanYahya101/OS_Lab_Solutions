#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/wait.h>
#include <math.h>

int main(int argc, char *argv[])
{
    if (argc < 2)
    {
        printf("Please pass in the conjugates as command line args\n");
        exit(0);
    }

    if (argc % 2 == 0)
    {
        printf("Please pass in an even number of conjugates\n");
        exit(0);
    }

    int arr[argc - 1];

    // convert all from 1 to end to ints
    for (int i = 1; i < argc; i++)
    {
        arr[i - 1] = atoi(argv[i]);
    }

    int pid1 = fork();

    if (pid1 == 0) // Child 1
    {
        // calculate complex conjugate
        for (int i = 0; i < argc - 1; i++)
        {
            if (i % 2 == 1)
            {
                arr[i] = arr[i] * -1;
            }
        }
        // create shared memory
        int shmid = shmget(1234, sizeof(arr), IPC_CREAT | 0666);
        if (shmid < 0)
        {
            printf("shmget error\n");
            exit(1);
        }

        int *shm = (int *)shmat(shmid, NULL, 0);
        if (shm == (int *)-1)
        {
            printf("shmat error\n");
            exit(1);
        }

        for (int i = 0; i < argc - 1; i++)
        {
            shm[i] = arr[i];
        }

        shmdt(shm);
        exit(0);
    }
    else
    {
        wait(NULL);
    }

    int pid2 = fork();

    if (pid2 == 0) // Child 2
    {
        // get `sqrt(a^2 + b^2)`. for each pair
        int arr2[(argc - 1) / 2];
        for (int i = 0; i < argc - 1; i += 2)
        {
            arr2[i / 2] = sqrt(pow(arr[i], 2) + pow(arr[i + 1], 2));
        }
        // store in a different shared memory block
        int shmid2 = shmget(5678, sizeof(arr2), IPC_CREAT | 0666);
        if (shmid2 < 0)
        {
            printf("shmget error\n");
            exit(1);
        }

        int *shm2 = (int *)shmat(shmid2, NULL, 0);
        if (shm2 == (int *)-1)
        {
            printf("shmat error\n");
            exit(1);
        }

        for (int i = 0; i < (argc - 1) / 2; i++)
        {
            shm2[i] = arr2[i];
        }

        shmdt(shm2);
        exit(0);
    }
    {
        wait(NULL);
    }

    int pid3 = fork();

    if (pid3 == 0) // Child 3
    {
        int real_final;
        int img_final;

        // add the real parts (ie even)
        real_final = 0;
        for (int i = 0; i < argc - 1; i += 2)
        {
            real_final += arr[i];
        }

        // add the imaginary parts (ie odd)
        img_final = 0;
        for (int i = 1; i < argc - 1; i += 2)
        {
            img_final += arr[i];
        }

        // save the real and imaginary final in a different shared block
        int shmid3 = shmget(91011, 2 * sizeof(int), IPC_CREAT | 0666);
        if (shmid3 < 0)
        {
            printf("shmget error\n");
            exit(1);
        }

        int *shm3 = (int *)shmat(shmid3, NULL, 0);
        if (shm3 == (int *)-1)
        {
            printf("shmat error\n");
            exit(1);
        }

        shm3[0] = real_final;
        shm3[1] = img_final;

        shmdt(shm3);

        exit(0);
    }
    else
    {
        // get all values from all 3 and print them
        // Retrieve and print the complex conjugates
        int shmid = shmget(1234, sizeof(int) * (argc - 1), 0666);
        if (shmid < 0)
        {
            printf("shmget error\n");
            exit(1);
        }

        int *shm = (int *)shmat(shmid, NULL, 0);
        if (shm == (int *)-1)
        {
            printf("shmat error\n");
            exit(1);
        }

        printf("Complex conjugates:\n");
        for (int i = 0; i < argc - 1; i = i + 2)
        {
            printf("%d%di ", shm[i], shm[i + 1]);
        }
        printf("\n");
        shmdt(shm);

        // Retrieve and print the magnitudes
        int shmid2 = shmget(5678, sizeof(int) * ((argc - 1) / 2), 0666);
        if (shmid2 < 0)
        {
            printf("shmget error\n");
            exit(1);
        }

        int *shm2 = (int *)shmat(shmid2, NULL, 0);
        if (shm2 == (int *)-1)
        {
            printf("shmat error\n");
            exit(1);
        }

        printf("Magnitudes:\n");
        for (int i = 0; i < (argc - 1) / 2; i++)
        {
            printf("%d ", shm2[i]);
        }
        printf("\n");
        shmdt(shm2);

        // Retrieve and print the final real and imaginary sums
        int shmid3 = shmget(91011, 2 * sizeof(int), 0666);
        if (shmid3 < 0)
        {
            printf("shmget error\n");
            exit(1);
        }

        int *shm3 = (int *)shmat(shmid3, NULL, 0);
        if (shm3 == (int *)-1)
        {
            printf("shmat error\n");
            exit(1);
        }

        printf("Final real sum: %d\n", shm3[0]);
        printf("Final imaginary sum: %d\n", shm3[1]);
        shmdt(shm3);
        wait(NULL);
        exit(0);
    }

    return 0;
}