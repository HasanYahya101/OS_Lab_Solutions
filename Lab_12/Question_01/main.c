#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/mman.h>

int main(int argc, char *argv[])
{
    if (argc != 2)
    {
        printf("Please enter filename as cmd arg\n");
        return 1;
    }

    int fd = open(argv[1], O_RDONLY);
    if (fd == -1)
    {
        printf("Error opening file\n");
        return 1;
    }

    off_t file_size = lseek(fd, 0, SEEK_END);
    if (file_size == -1)
    {
        printf("Error getting file size\n");
        close(fd);
        return 1;
    }

    if (lseek(fd, 0, SEEK_SET) == -1)
    {
        printf("Error seeking to start of file\n");
        close(fd);
        return 1;
    }

    void *mapped = mmap(NULL, file_size, PROT_READ, MAP_PRIVATE, fd, 0);
    if (mapped == MAP_FAILED)
    {
        printf("Error mapping file\n");
        close(fd);
        return 1;
    }

    // Print first 100 bytes (or less if file is smaller)
    size_t bytes_to_print = file_size < 100 ? file_size : 100;
    write(1, mapped, bytes_to_print); // to stdout
    printf("\n");

    if (munmap(mapped, file_size) == -1)
    {
        printf("Error unmapping file\n");
    }
    close(fd);

    return 0;
}