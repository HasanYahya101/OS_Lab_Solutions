#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/mman.h>
#include <sys/types.h>

int count_occurrences(const char *mapped, size_t file_size, const char *word)
{
    int count = 0;
    size_t word_len = strlen(word);

    for (size_t i = 0; i <= file_size - word_len; i++)
    {
        if (strncmp(mapped + i, word, word_len) == 0)
        {
            count++;
            i += word_len - 1;
        }
    }

    return count;
}

int main(int argc, char *argv[])
{
    if (argc != 3)
    {
        printf("Enter filename and words as  cmd args");
        return 1;
    }

    const char *filename = argv[1];
    const char *word = argv[2];

    int fd = open(filename, O_RDONLY);
    if (fd == -1)
    {
        printf("Error opening file");
        return 1;
    }

    off_t file_size = lseek(fd, 0, SEEK_END);
    if (file_size == -1)
    {
        printf("Error getting file size");
        close(fd);
        return 1;
    }

    void *mapped = mmap(NULL, file_size, PROT_READ, MAP_PRIVATE, fd, 0);
    if (mapped == MAP_FAILED)
    {
        printf("Error mapping file");
        close(fd);
        return 1;
    }

    close(fd);

    int occurrences = count_occurrences((const char *)mapped, file_size, word);

    printf("The word \"%s\" was found %d times.\n", word, occurrences);

    if (munmap(mapped, file_size) == -1)
    {
        printf("Error unmapping file");
        return 1;
    }

    return 0;
}
