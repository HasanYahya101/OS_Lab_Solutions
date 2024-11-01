#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>

void *worker(void *arg)
{
    (void)arg; // Indicate argument is unused
    pthread_t current_thread_id = pthread_self();
    pid_t current_process_id = getpid();
    printf("Thread ID: %lu, Process ID: %d\n", (unsigned long)current_thread_id, current_process_id);
    return NULL;
}

int main(int argc, char *argv[])
{
    if (argc != 2)
    {
        printf("Please provide the number of threads as an argument.\n");
        return 1;
    }

    int thread_count = atoi(argv[1]);
    if (thread_count <= 0)
    {
        printf("Invalid number of threads. Must be greater than 0.\n");
        return 1;
    }

    pthread_t threads[thread_count];

    for (int i = 0; i < thread_count; i++)
    {
        if (pthread_create(&threads[i], NULL, worker, NULL) != 0)
        {
            printf("Error: Could not create thread %d.\n", i + 1);
            return 1;
        }
    }

    for (int i = 0; i < thread_count; i++)
    {
        pthread_join(threads[i], NULL);
    }

    return 0;
}
