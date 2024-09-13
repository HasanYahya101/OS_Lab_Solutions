#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

void combine(int array[], int start, int middle, int end)
{
    int leftSize = middle - start + 1;
    int rightSize = end - middle;

    int left[leftSize], right[rightSize];

    for (int i = 0; i < leftSize; i++)
        left[i] = array[start + i];
    for (int j = 0; j < rightSize; j++)
        right[j] = array[middle + 1 + j];

    int i = 0, j = 0, k = start;
    while (i < leftSize && j < rightSize)
    {
        if (left[i] <= right[j])
        {
            array[k] = left[i];
            i++;
        }
        else
        {
            array[k] = right[j];
            j++;
        }
        k++;
    }

    while (i < leftSize)
    {
        array[k] = left[i];
        i++;
        k++;
    }

    while (j < rightSize)
    {
        array[k] = right[j];
        j++;
        k++;
    }
}

void sort(int array[], int start, int end)
{
    if (start < end)
    {
        int middle = start + (end - start) / 2;

        sort(array, start, middle);
        sort(array, middle + 1, end);

        combine(array, start, middle, end);
    }
}

int main(int argc, char *argv[])
{
    if (argc != 2)
    {
        printf("Enter filename in command args (error)\n");
        return 1;
    }

    FILE *inputFile = fopen(argv[1], "r");
    if (!inputFile)
    {
        printf("Failed to open file");
        return 1;
    }

    int numbers[1000];
    int count = 0;

    while (fscanf(inputFile, "%d", &numbers[count]) != -1) // -1 here is end of file
    {
        count++;
    }
    fclose(inputFile);

    int half = count / 2;

    FILE *firstHalf = fopen("first_half.txt", "w");
    FILE *secondHalf = fopen("second_half.txt", "w");

    for (int i = 0; i < half; i++)
    {
        fprintf(firstHalf, "%d\n", numbers[i]);
    }
    for (int i = half; i < count; i++)
    {
        fprintf(secondHalf, "%d\n", numbers[i]); // print to file
    }

    fclose(firstHalf);
    fclose(secondHalf);

    pid_t pid1 = fork();
    if (pid1 == 0)
    {
        execlp("sort", "sort", "-n", "first_half.txt", "-o", "sorted_first_half.txt", NULL);
        printf("Failed to execute sort for first half");
        exit(1);
    }

    pid_t pid2 = fork();
    if (pid2 == 0)
    {
        execlp("sort", "sort", "-n", "second_half.txt", "-o", "sorted_second_half.txt", NULL);
        printf("Failed to execute sort for second half");
        exit(1);
    }

    wait(NULL);
    wait(NULL);

    FILE *sortedFirst = fopen("sorted_first_half.txt", "r");
    FILE *sortedSecond = fopen("sorted_second_half.txt", "r");

    int sortedNumbers[1000];
    int i = 0, j = 0, k = 0;

    while (fscanf(sortedFirst, "%d", &sortedNumbers[i]) != -1)
    {
        i++;
    }
    while (fscanf(sortedSecond, "%d", &sortedNumbers[half + j]) != -1)
    {
        j++;
    }

    fclose(sortedFirst);
    fclose(sortedSecond);

    sort(sortedNumbers, 0, count - 1);

    FILE *outputFile = fopen("final_sorted_numbers.txt", "w");
    for (int i = 0; i < count; i++)
    {
        fprintf(outputFile, "%d\n", sortedNumbers[i]); // output to file
    }
    fclose(outputFile);

    return 0;
}
