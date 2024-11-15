#include <stdio.h>
#include <pthread.h>
#include <unistd.h>

#define ITERATIONS 10

pthread_mutex_t mutex_A = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t mutex_B = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t mutex_C = PTHREAD_MUTEX_INITIALIZER;

void *printA(void *arg)
{
    for (int i = 0; i < ITERATIONS; i++)
    {
        pthread_mutex_lock(&mutex_A);

        printf("A");

        pthread_mutex_unlock(&mutex_B); // Signal B
        usleep(100);
    }
    return NULL;
}

void *printB(void *arg)
{
    for (int i = 0; i < ITERATIONS; i++)
    {
        pthread_mutex_lock(&mutex_B);

        printf("B");

        pthread_mutex_unlock(&mutex_C); // Signal C
        usleep(100);
    }
    return NULL;
}

void *printC(void *arg)
{
    for (int i = 0; i < ITERATIONS; i++)
    {
        pthread_mutex_lock(&mutex_C);

        printf("C");

        pthread_mutex_unlock(&mutex_A); // Signal A
        usleep(100);
    }
    return NULL;
}

int main()
{
    pthread_t thread1, thread2, thread3;

    pthread_mutex_lock(&mutex_B);
    pthread_mutex_lock(&mutex_C);

    pthread_create(&thread1, NULL, printA, NULL);
    pthread_create(&thread2, NULL, printB, NULL);
    pthread_create(&thread3, NULL, printC, NULL);

    // Small delay
    usleep(100);

    pthread_join(thread1, NULL);
    pthread_join(thread2, NULL);
    pthread_join(thread3, NULL);

    printf("\n");

    pthread_mutex_destroy(&mutex_A);
    pthread_mutex_destroy(&mutex_B);
    pthread_mutex_destroy(&mutex_C);

    return 0;
}
