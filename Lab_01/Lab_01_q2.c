#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(int argc, char *argv[])
{
    if (argc > 1)
    {
        char *courseName = (char *)malloc(strlen(argv[1]) + 1);

        for (int i = 0; i < strlen(argv[1]); i++)
        {
            courseName[i] = argv[1][i];
        }

        printf("Welcome to BSBS Operating System Lab %s\n", courseName);
    }
    else
    {
        printf("Error: Please enter the name of the course\n");
    }
    return 0;
}