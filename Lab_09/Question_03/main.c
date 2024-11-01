#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

typedef struct
{
    int n;
    int *fib;
    int even_count;
    int odd_count;
    int sum;
} fib_data;

void *fibonacci(void *arg)
{
    fib_data *data = (fib_data *)arg;
    data->fib = (int *)malloc(data->n * sizeof(int));
    if (!data->fib)
    {
        fprintf(stderr, "Memory allocation failed\n");
        pthread_exit(NULL);
    }

    data->fib[0] = 0;
    if (data->n > 1)
    {
        data->fib[1] = 1;
    }

    for (int i = 2; i < data->n; i++)
    {
        data->fib[i] = data->fib[i - 1] + data->fib[i - 2];
    }
    return NULL;
}

void *countEven(void *arg)
{
    fib_data *data = (fib_data *)arg;
    data->even_count = 0;
    for (int i = 0; i < data->n; i++)
    {
        if (data->fib[i] % 2 == 0)
        {
            data->even_count++;
        }
    }
    return NULL;
}

void *countOdd(void *arg)
{
    fib_data *data = (fib_data *)arg;
    data->odd_count = 0;
    for (int i = 0; i < data->n; i++)
    {
        if (data->fib[i] % 2 != 0)
        {
            data->odd_count++;
        }
    }
    return NULL;
}

void *calcSum(void *arg)
{
    fib_data *data = (fib_data *)arg;
    data->sum = 0;
    for (int i = 0; i < data->n; i++)
    {
        data->sum += data->fib[i];
    }

    FILE *file = fopen("sum.txt", "w");
    if (file)
    {
        fprintf(file, "Sum: %d\n", data->sum);
        fclose(file);
    }
    return NULL;
}

int main(int argc, char *argv[])
{
    if (argc != 2)
    {
        printf("Please pass a number as cmd arg\n");
        return 1;
    }

    int N = atoi(argv[1]);
    if (N <= 0)
    {
        printf("Number passed as cmd arg must be positive\n");
        return 1;
    }

    printf("Number entered is %d\n", N);

    fib_data data;
    data.n = N;
    pthread_t threads[4];

    pthread_create(&threads[0], NULL, fibonacci, &data);
    pthread_join(threads[0], NULL);
    printf("ID = %lu, Series: ", threads[0]);
    for (int i = 0; i < N; i++)
    {
        printf("%d ", data.fib[i]);
    }
    printf("\n");

    pthread_create(&threads[1], NULL, countEven, &data);
    pthread_create(&threads[2], NULL, countOdd, &data);
    pthread_create(&threads[3], NULL, calcSum, &data);
    pthread_join(threads[1], NULL);
    printf("ID = %lu, Even Numbers: %d\n", threads[1], data.even_count);
    pthread_join(threads[2], NULL);
    printf("ID = %lu, Odd Numbers: %d\n", threads[2], data.odd_count);
    pthread_join(threads[3], NULL);
    printf("ID = %lu, Sum: %d\n", threads[3], data.sum);

    free(data.fib);

    return 0;
}
