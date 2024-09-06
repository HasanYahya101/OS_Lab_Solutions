#include <stdbool.h>
#include <stdio.h>
#include "header.h"

void sort(int array[], bool order, int size)
{
    int n = size;
    if (order == true) // sort in ascending order
    {
        for (int i = 0; i < n - 1; i++)
        {
            int min = i;

            for (int j = i + 1; j < n; j++)
            {
                if (array[j] < array[min])
                {
                    min = j;
                }
            }

            int temp = array[min];
            array[min] = array[i];
            array[i] = temp;
        }
        printf("Sorted in ascending order\n");
    }
    else
    {
        // sort in descending order
        for (int i = 0; i < n - 1; i++)
        {
            int min = i;

            for (int j = i + 1; j < n; j++)
            {
                if (array[j] > array[min])
                {
                    min = j;
                }
            }

            int temp = array[min];
            array[min] = array[i];
            array[i] = temp;
        }
        printf("Sorted in decending order\n");
    }
}

void findHighest(int array[], int nth, int size)
{
    sort(array, false, size); // descending
    int highest = array[0];
    int count = 1;

    for (int i = 1; i < size; i++)
    {
        if (array[i] != highest)
        {
            highest = array[i];
            count++;
        }

        if (count == nth)
        {
            break;
        }
    }

    if (count == nth)
    {
        printf("%d highest number is: %d\n", nth, highest);
    }
    else
    {
        printf("There are less than %d distinct numbers in the array.\n", nth);
    }
}

void print(int print[], int size)
{
    // print all elements in an array
    printf("The array values are as under:\n");
    for (int i = 0; i < size; i++)
    {
        printf("%d\n", print[i]);
    }
}