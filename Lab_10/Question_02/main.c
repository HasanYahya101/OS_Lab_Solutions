#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>

#define ARRAY_SIZE 5
#define TOTAL_SHIPMENTS 10
#define RATE 15

struct SharedData
{
    int shipments[ARRAY_SIZE];
    int in;
    int out;
    int count;
    sem_t empty;
    sem_t full;
    sem_t mutex;
};

struct SharedData shared;
int processed_count = 0;

void *verifier(void *arg)
{
    int shipment_id = 1001;

    while (processed_count < TOTAL_SHIPMENTS)
    {
        sem_wait(&shared.empty);
        sem_wait(&shared.mutex);

        // Add shipment to array
        if (shared.count < ARRAY_SIZE)
        {
            shared.shipments[shared.in] = shipment_id;
            shared.in = (shared.in + 1) % ARRAY_SIZE;
            shared.count++;
            shipment_id++;
        }

        sem_post(&shared.mutex);
        sem_post(&shared.full);

        usleep(100000);
    }
    return NULL;
}

void *processor(void *arg)
{
    while (processed_count < TOTAL_SHIPMENTS)
    {
        sem_wait(&shared.full);
        sem_wait(&shared.mutex);

        if (shared.count > 0)
        {
            int id = shared.shipments[shared.out];
            shared.out = (shared.out + 1) % ARRAY_SIZE;
            shared.count--;
            processed_count++;

            int invoice = id * RATE;
            printf("Shipment ID: %d, Invoice: $%d\n", id, invoice);
        }

        sem_post(&shared.mutex);
        sem_post(&shared.empty);

        usleep(150000); // sleep for 0.15 seconds
    }
    return NULL;
}

int main()
{
    shared.in = 0;
    shared.out = 0;
    shared.count = 0;

    sem_init(&shared.empty, 0, ARRAY_SIZE);
    sem_init(&shared.full, 0, 0);
    sem_init(&shared.mutex, 0, 1);

    pthread_t verifier_thread, processor_thread;
    pthread_create(&verifier_thread, NULL, verifier, NULL);
    pthread_create(&processor_thread, NULL, processor, NULL);

    pthread_join(verifier_thread, NULL);
    pthread_join(processor_thread, NULL);

    sem_destroy(&shared.empty);
    sem_destroy(&shared.full);
    sem_destroy(&shared.mutex);

    printf("Processing complete. Total shipments processed: %d\n", processed_count);
    return 0;
}