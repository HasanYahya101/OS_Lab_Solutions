#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>
#include <time.h>

sem_t resource;
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
int waiting_threads[10] = {0};
int available_resources = 5;

typedef struct
{
    int id;
    int time;
} ThreadInfo;

void *thread_function(void *arg)
{
    ThreadInfo *info = (ThreadInfo *)arg;
    int resources_needed = (rand() % 3) + 1;

    printf("Thread %d requested %d resources.\n", info->id, resources_needed);

    // Record waiting time
    pthread_mutex_lock(&mutex);
    waiting_threads[info->id] = time(NULL);
    pthread_mutex_unlock(&mutex);

    for (int i = 0; i < resources_needed; i++)
    {
        sem_wait(&resource);
    }

    pthread_mutex_lock(&mutex);
    available_resources -= resources_needed;
    printf("Thread %d acquired %d resources. (Available: %d)\n",
           info->id, resources_needed, available_resources);
    pthread_mutex_unlock(&mutex);

    sleep(1); // 1 sec

    pthread_mutex_lock(&mutex);
    available_resources += resources_needed;
    printf("Thread %d released %d resources. (Available: %d)\n",
           info->id, resources_needed, available_resources);
    pthread_mutex_unlock(&mutex);

    for (int i = 0; i < resources_needed; i++)
    {
        sem_post(&resource);
    }

    free(info);
    return NULL;
}

int main()
{
    pthread_t threads[10];
    srand(time(NULL));

    sem_init(&resource, 0, 5);

    for (int i = 0; i < 10; i++)
    {
        ThreadInfo *info = malloc(sizeof(ThreadInfo));
        info->id = i;
        info->time = time(NULL);

        if (pthread_create(&threads[i], NULL, thread_function, (void *)info) != 0)
        {
            perror("Failed to create thread");
            return 1;
        }

        usleep(100000); // 100ms
    }

    for (int i = 0; i < 10; i++)
    {
        pthread_join(threads[i], NULL);
    }

    sem_destroy(&resource);
    pthread_mutex_destroy(&mutex);

    return 0;
}