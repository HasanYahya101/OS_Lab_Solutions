#include <stdio.h>
#include <pthread.h>
#include <semaphore.h>
#include <stdlib.h>
#include <unistd.h>

sem_t sem_S1_done;
sem_t sem_S2_done;
sem_t log_mutex;

FILE *log_file;

void *service_S1(void *arg)
{
    sem_wait(&log_mutex);

    printf("S1: Placing order\n");
    log_file = fopen("shared_log.txt", "a");
    fprintf(log_file, "Order ID: 1234, Customer: John Doe\n");
    fclose(log_file);

    sem_post(&log_mutex);
    sem_post(&sem_S1_done);

    return NULL;
}

void *service_S2(void *arg)
{
    sem_wait(&sem_S1_done);
    sem_wait(&log_mutex);

    printf("S2: Processing payment\n");
    log_file = fopen("shared_log.txt", "a");
    fprintf(log_file, "Payment Status: Successful\n");
    fclose(log_file);

    sem_post(&log_mutex);
    sem_post(&sem_S2_done);

    return NULL;
}

void *service_S3(void *arg)
{
    sem_wait(&sem_S2_done);
    sem_wait(&log_mutex);

    printf("S3: Confirming order\n");
    log_file = fopen("shared_log.txt", "a");
    fprintf(log_file, "Order Confirmation: Confirmed, Timestamp: %ld\n", time(NULL));
    fclose(log_file);

    sem_post(&log_mutex);

    return NULL;
}

int main()
{
    sem_init(&sem_S1_done, 0, 0);
    sem_init(&sem_S2_done, 0, 0);
    sem_init(&log_mutex, 0, 1);

    pthread_t thread_S1, thread_S2, thread_S3;

    pthread_create(&thread_S1, NULL, service_S1, NULL);
    pthread_create(&thread_S2, NULL, service_S2, NULL);
    pthread_create(&thread_S3, NULL, service_S3, NULL);

    pthread_join(thread_S1, NULL);
    pthread_join(thread_S2, NULL);
    pthread_join(thread_S3, NULL);

    sem_destroy(&sem_S1_done);
    sem_destroy(&sem_S2_done);
    sem_destroy(&log_mutex);

    printf("All services completed successfully.\n");
    return 0;
}