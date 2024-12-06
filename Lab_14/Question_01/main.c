#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <fcntl.h>
#include <sys/wait.h>

union semun
{
    int val;
    struct semid_ds *buf;
    unsigned short *array;
};

int init_semaphores()
{
    key_t key = ftok(".", 'S');
    int semid = semget(key, 2, IPC_CREAT | 0666);

    union semun sem_arg;
    sem_arg.val = 1;
    semctl(semid, 0, SETVAL, sem_arg);

    sem_arg.val = 0;
    semctl(semid, 1, SETVAL, sem_arg);

    return semid;
}

// P1
void calc_sum(int semid)
{
    struct sembuf sb = {0, -1, 0}; // Wait operation
    semop(semid, &sb, 1);          // Lock file access

    FILE *file = fopen("numbers.txt", "r+");
    if (file == NULL)
    {
        printf("File opening failed");
        exit(1);
    }

    int num, sum = 0;
    while (fscanf(file, "%d", &num) == 1)
    {
        sum += num;
    }

    // Write sum at the end of file
    fseek(file, 0, SEEK_END);
    fprintf(file, "\nSum: %d", sum);
    fclose(file);

    sb.sem_num = 1;
    sb.sem_op = 1; // Signal P2 to start
    semop(semid, &sb, 1);

    printf("P1: Sum calculation completed\n");
}

// P2
void count_int(int semid)
{
    struct sembuf sb = {1, -1, 0}; // Wait for P1
    semop(semid, &sb, 1);

    sb.sem_num = 0;
    sb.sem_op = -1; // Lock file access
    semop(semid, &sb, 1);

    FILE *file = fopen("numbers.txt", "r+");
    if (file == NULL)
    {
        printf("File opening failed");
        exit(1);
    }

    int num, count = 0;
    while (fscanf(file, "%d", &num) == 1)
    {
        count++;
    }

    fseek(file, 0, SEEK_END);
    fprintf(file, "\nCount: %d", count);
    fclose(file);

    sb.sem_num = 1;
    sb.sem_op = 1; // Signal P3 to start
    semop(semid, &sb, 1);

    printf("P2: Count calculation completed\n");
}

// P3
void calc_avg(int semid)
{
    struct sembuf sb = {1, -1, 0}; // Wait for P2
    semop(semid, &sb, 1);

    FILE *file = fopen("numbers.txt", "r");
    if (file == NULL)
    {
        printf("File opening failed");
        exit(1);
    }

    int sum, count;
    char line[100];
    while (fgets(line, sizeof(line), file))
    {
        if (sscanf(line, "Sum: %d", &sum) == 1)
            continue;
        if (sscanf(line, "Count: %d", &count) == 1)
            break;
    }
    fclose(file);

    float average = (float)sum / count;
    printf("P3: Average = %.2f\n", average);

    semctl(semid, 0, IPC_RMID, 0);
}

int main()
{
    int semid = init_semaphores();
    pid_t pid1, pid2;

    pid1 = fork();
    if (pid1 == 0)
    {
        calc_sum(semid);
        exit(0);
    }

    pid2 = fork();
    if (pid2 == 0)
    {
        count_int(semid);
        exit(0);
    }

    calc_avg(semid); // P3

    wait(NULL);
    wait(NULL);

    return 0;
}