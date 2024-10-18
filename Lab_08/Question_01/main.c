#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/wait.h>

#define FIFO_NAME "/tmp/fifo"
#define BUFFER_SIZE 1024

int count_vowels(const char *word)
{
    int count = 0;
    while (*word)
    {
        if (strchr("AEIOUaeiou", *word))
        {
            count++;
        }
        word++;
    }
    return count;
}

void find_word_with_most_vowels(const char *sentence, char *result)
{
    char *token;
    char *sentence_copy = strdup(sentence);
    int max_vowels = 0;

    token = strtok(sentence_copy, " ");
    while (token != NULL)
    {
        int vowels = count_vowels(token);
        if (vowels > max_vowels)
        {
            max_vowels = vowels;
            strcpy(result, token);
        }
        token = strtok(NULL, " ");
    }

    free(sentence_copy);
}

int main(int argc, char *argv[])
{
    if (argc != 2)
    {
        printf("Error: Pass in filename as cmd argument\n");
        exit(1);
    }

    if (access(FIFO_NAME, F_OK) == 0)
    {
        printf("FIFO already exists.\n");
    }
    else
    {
        if (mkfifo(FIFO_NAME, 0666) == -1)
        {
            printf("Error creating FIFO");
            exit(1);
        }
        else
        {
            printf("FIFO created successfully.\n");
        }
    }

    pid_t pid1, pid2;
    int fd;
    if ((pid1 = fork()) == 0)
    {
        FILE *file = fopen(argv[1], "r");
        if (!file)
        {
            printf("Error: File not found\n");
            exit(1);
        }
        else
        {
            printf("File opened successfully\n");
        }

        fd = open(FIFO_NAME, O_WRONLY);
        if (fd == -1)
        {
            printf("Error: File not opening\n");
            exit(1);
        }
        else
        {
            printf("FIFO opened successfully\n");
        }

        char buffer[BUFFER_SIZE];
        while (fgets(buffer, BUFFER_SIZE, file))
        {
            write(fd, buffer, strlen(buffer));
        }

        const char end_marker[] = "END";
        write(fd, end_marker, sizeof(end_marker));

        close(fd);
        fclose(file);
        exit(0);
    }

    if ((pid2 = fork()) == 0)
    {
        fd = open(FIFO_NAME, O_RDONLY);
        if (fd == -1)
        {
            printf("Error: File not opening\n");
            exit(1);
        }

        char buffer[BUFFER_SIZE];
        char content[BUFFER_SIZE * 10] = "";
        ssize_t bytes_read;

        while ((bytes_read = read(fd, buffer, BUFFER_SIZE - 1)) > 0)
        {
            buffer[bytes_read] = '\0';
            if (strcmp(buffer, "END") == 0)
            {
                break;
            }
            strcat(content, buffer);
        }

        close(fd);

        char *sentence = strtok(content, ".");
        char longest_sentence[BUFFER_SIZE] = "";

        while (sentence != NULL)
        {
            if (strlen(sentence) > strlen(longest_sentence))
            {
                strcpy(longest_sentence, sentence);
            }
            sentence = strtok(NULL, ".");
        }

        char word_with_most_vowels[BUFFER_SIZE] = "";
        find_word_with_most_vowels(longest_sentence, word_with_most_vowels);

        FILE *result_file = fopen("result.txt", "w");
        if (!result_file)
        {
            printf("Error: File not opening\n");
            exit(1);
        }

        fprintf(result_file, "Longest sentence: %s\n", longest_sentence);
        fprintf(result_file, "Word with the most vowels: %s\n", word_with_most_vowels);
        fclose(result_file);

        exit(0);
    }

    waitpid(pid1, NULL, 0);
    waitpid(pid2, NULL, 0);

    FILE *result_file = fopen("result.txt", "r");
    if (!result_file)
    {
        printf("Error: File not opening\n");
        exit(1);
    }

    char result_buffer[BUFFER_SIZE];
    while (fgets(result_buffer, BUFFER_SIZE, result_file))
    {
        printf("%s", result_buffer);
    }

    fclose(result_file);
    unlink(FIFO_NAME);

    return 0;
}
