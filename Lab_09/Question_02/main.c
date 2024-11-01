#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

typedef struct
{
    int n;
    int result;
} number;

void *summation(void *args)
{
    number *arguments = (number *)args;
    int n = arguments->n;
    int sum = 0;
    for (int i = 0; i <= n; i++)
    {
        sum += i;
    }
    arguments->result = sum;
    return NULL;
}

int main()
{
    int num;
    printf("Enter a positive integer: ");
    scanf("%d", &num);

    while (num < 0)
    {
        printf("Error! Please enter a positive integer: ");
        scanf("%d", &num);
    }

    number args;
    args.n = num;
    pthread_t thread;
    if (pthread_create(&thread, NULL, summation, &args) != 0)
    {
        fprintf(stderr, "Error creating thread\n");
        return 1;
    }
    pthread_join(thread, NULL);
    printf("The summation of integers from 0 to %d is: %d\n", num, args.result);

    return 0;
}
