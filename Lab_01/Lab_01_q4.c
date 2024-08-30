#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>

int main(int argc, char *argv[])
{
    if (argc == 3)
    {
        FILE *inputFile, *outputFile;
        inputFile = fopen(argv[1], "r");
        outputFile = fopen(argv[2], "w");

        if (inputFile == NULL)
        {
            printf("Error: Input file not found\n");
            return 1;
        }

        if (outputFile == NULL)
        {
            printf("Error: Output file cannot be created\n");
            return 1;
        }

        char ch;
        while ((ch = fgetc(inputFile)) != EOF)
        {
            if (isdigit(ch))
            {
                fputc(ch, outputFile);
                fputc(' ', outputFile); // put a space after each digit
            }
        }

        fclose(inputFile);
        fclose(outputFile);
    }
    else
    {
        printf("Error: Please pass input and output file names as arguments\n");
    }

    return 0;
}
