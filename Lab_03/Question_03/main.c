#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <fcntl.h>
#include <unistd.h>

struct Word
{
    char word[100];
    int freq;
};

void addWord(struct Word words[], int *n, char *word)
{
    for (int i = 0; i < *n; i++)
    {
        if (strcmp(words[i].word, word) == 0)
        {
            words[i].freq++;
            return;
        }
    }

    strcpy(words[*n].word, word);
    words[*n].freq = 1;
    (*n)++;
}

int is_aphabet(char alpha)
{
    return (alpha >= 'A' && alpha <= 'Z') || (alpha >= 'a' && alpha <= 'z');
}

int main(int argc, char *argv[])
{
    if (argc < 2)
    {
        printf("Error: Please enter one or more filenames\n");
    }
    printf("\n");
    struct Word words[1000];
    int n = 0;
    char ch, buffer[100];
    int index, word_index;

    for (index = 1; index < argc; index++)
    {
        int fd = open(argv[index], 00);
        if (fd == -1)
        {
            printf("Failed to open file %s\n", argv[index]);
            exit(-1);
        }

        word_index = 0;
        while (read(fd, &ch, 1) == 1)
        {
            if (is_aphabet(ch))
            {
                buffer[word_index++] = tolower(ch);
            }
            else if (word_index != 0)
            {
                buffer[word_index] = '\0';
                addWord(words, &n, buffer);
                word_index = 0;
            }
        }

        close(fd);
    }

    int i, j, max;
    for (i = 0; i < 10 && i < n; i++)
    {
        max = i;
        for (j = i + 1; j < n; j++)
        {
            if (words[j].freq > words[max].freq)
            {
                max = j;
            }
        }
        struct Word temp = words[i];
        words[i] = words[max];
        words[max] = temp;
        printf("%s: ", words[i].word);
        printf("%d\n", words[i].freq);
    }

    return 0;
}