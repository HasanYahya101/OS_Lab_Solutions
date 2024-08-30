#include <stdio.h>
#include <stdlib.h> // to convert from string to int

int main(int argc, char *argv[])
{
    if (argc > 1)
    {
        int sum = 0;
        int avg = 0;
        for (int i = 1; i < argc; i++)
        {
            sum += atoi(argv[i]);
        }
        avg = sum / (argc - 1);
        printf("The Sum is: %d\n", sum);
        printf("The Average is: %d\n", avg);
    }
    else
    {
        printf("Error: Please pass an argument\n");
    }
    return 0;
}