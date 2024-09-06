#include <stdio.h>
#include <stdlib.h>
#include "header.h"

int main(int argc, char *argv[])
{
    if (argc > 1)
    {
        const int arr_size = argc - 3;
        if (arr_size < 1)
        {
            printf("Error: Please enter a valid array");
            exit(1);
        }

        int arr[arr_size];

        for (int i = 0; i < arr_size; i++)
        {
            arr[i] = atoi(argv[i + 1]);
        }

        int flag = atoi(argv[argc - 2]);
        if (flag != 0 && flag != 1)
        {
            printf("Enter valid flag\n");
        }
        int nth = atoi(argv[argc - 1]);
        if (nth < 1)
        {
            printf("error enter valid nth val\n");
            exit(0);
        }
        sort(arr, flag, arr_size);
        print(arr, arr_size);
        findHighest(arr, nth, arr_size);
    }
    else
    {
        printf("No arguments provided.\n");
    }
    return 0;
}