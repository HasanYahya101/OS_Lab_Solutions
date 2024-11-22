#include <iostream>
#include <fstream>
#include <string>
#include <cstring>
#include <sys/mman.h>
#include <fcntl.h>
#include <unistd.h>

using namespace std;

void replaceWordInFile(const string &fileName, const string &wordToReplace, const string &replacementWord)
{
    int fd = open(fileName.c_str(), O_RDWR);
    if (fd == -1)
    {
        printf("Error opening file: %s\n", fileName.c_str());
        return;
    }

    off_t file_size = lseek(fd, 0, SEEK_END);
    if (file_size == -1)
    {
        printf("Error getting file size for file: %s\n", fileName.c_str());
        close(fd);
        return;
    }

    void *mapped = mmap(NULL, file_size, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
    if (mapped == MAP_FAILED)
    {
        printf("Error mapping file: %s\n", fileName.c_str());
        close(fd);
        return;
    }

    close(fd);

    size_t word_len = wordToReplace.size();
    size_t replacement_len = replacementWord.size();

    if (word_len != replacement_len)
    {
        printf("Warning: Replacing words of different lengths may cause unexpected behavior.\n");
    }

    for (size_t i = 0; i <= file_size - word_len; i++)
    {
        if (strncmp((const char *)mapped + i, wordToReplace.c_str(), word_len) == 0)
        {
            memcpy((char *)mapped + i, replacementWord.c_str(), replacement_len);

            if (replacement_len < word_len)
            {
                memmove((char *)mapped + i + replacement_len, (char *)mapped + i + word_len, file_size - (i + word_len));
                file_size -= (word_len - replacement_len);
            }
            i += replacement_len - 1;
        }
    }

    // Print the file contents after replacement
    printf("\nFile contents after replacement:\n");
    printf("--------------------------------\n");
    write(STDOUT_FILENO, mapped, file_size);
    printf("\n--------------------------------\n");

    if (msync(mapped, file_size, MS_SYNC) == -1)
    {
        printf("Error syncing file changes for file: %s\n", fileName.c_str());
    }

    if (munmap(mapped, file_size) == -1)
    {
        printf("Error unmapping file: %s\n", fileName.c_str());
    }
}

int main(int argc, char *argv[])
{
    if (argc != 4)
    {
        printf("Enter filename and words as cmd args");
        return 1;
    }

    string fileName = argv[1];
    string wordToReplace = argv[2];
    string replacementWord = argv[3];

    replaceWordInFile(fileName, wordToReplace, replacementWord);

    return 0;
}
