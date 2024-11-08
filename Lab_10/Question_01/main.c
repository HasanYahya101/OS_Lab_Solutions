#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>
#include <time.h>

#define SIZE 10
#define PRODUCERS 3
#define CONSUMERS 2
#define ITEMS 50

int buffer[SIZE];
int in = 0;
int out = 0;
int items_produced = 0;
int items_consumed = 0;

sem_t empty;
sem_t full;
sem_t mutex;

void *producer(void *arg)
{
    int producer_id = *((int *)arg);

    while (1)
    {
        sem_wait(&mutex);
        if (items_produced >= ITEMS)
        {
            sem_post(&mutex);
            break;
        }
        sem_post(&mutex);

        sem_wait(&empty);
        sem_wait(&mutex);

        if (items_produced >= ITEMS)
        {
            sem_post(&mutex);
            sem_post(&empty);
            break;
        }

        int item = rand() % 100;
        buffer[in] = item;
        printf("Producer %d produced: %d at position %d\n", producer_id, item, in);
        printf("Total items produced: %d\n", items_produced + 1);
        in = (in + 1) % SIZE;
        items_produced++;

        sem_post(&mutex);
        sem_post(&full);

        usleep(rand() % 1000000);
    }
    return NULL;
}

void *consumer(void *arg)
{
    int consumer_id = *((int *)arg);

    while (1)
    {
        sem_wait(&mutex);
        if (items_consumed >= ITEMS)
        {
            sem_post(&mutex);
            break;
        }
        sem_post(&mutex);

        sem_wait(&full);
        sem_wait(&mutex);

        if (items_consumed >= ITEMS)
        {
            sem_post(&mutex);
            sem_post(&full);
            break;
        }

        int item = buffer[out];
        printf("Consumer %d consumed: %d from position %d\n", consumer_id, item, out);
        printf("Total items consumed: %d\n", items_consumed + 1);
        out = (out + 1) % SIZE;
        items_consumed++;

        sem_post(&mutex);
        sem_post(&empty);

        usleep(rand() % 1000000);
    }
    return NULL;
}

int main()
{
    srand(time(NULL));

    sem_init(&empty, 0, SIZE);
    sem_init(&full, 0, 0);
    sem_init(&mutex, 0, 1);
    pthread_t producers[PRODUCERS];
    pthread_t consumers[CONSUMERS];
    int producer_ids[PRODUCERS];
    int consumer_ids[CONSUMERS];

    for (int i = 0; i < PRODUCERS; i++)
    {
        producer_ids[i] = i + 1;
        pthread_create(&producers[i], NULL, producer, &producer_ids[i]);
    }

    for (int i = 0; i < CONSUMERS; i++)
    {
        consumer_ids[i] = i + 1;
        pthread_create(&consumers[i], NULL, consumer, &consumer_ids[i]);
    }

    for (int i = 0; i < PRODUCERS; i++)
    {
        pthread_join(producers[i], NULL);
    }
    for (int i = 0; i < CONSUMERS; i++)
    {
        pthread_join(consumers[i], NULL);
    }

    sem_destroy(&empty);
    sem_destroy(&full);
    sem_destroy(&mutex);

    printf("All items have been produced and consumed.\n");
    return 0;
}