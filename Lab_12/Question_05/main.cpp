#include <iostream>
#include <string>
#include <pthread.h>
#include <sys/mman.h>
#include <fcntl.h>
#include <unistd.h>
#include <cstring>
using namespace std;

struct ThreadData
{
    void *mapped;
    size_t start;
    size_t length;
};

void *convertToLowercase(void *arg)
{
    ThreadData *data = static_cast<ThreadData *>(arg);
    char *mapped = static_cast<char *>(data->mapped);
    size_t start = data->start;
    size_t length = data->length;

    for (size_t i = start; i < start + length; ++i)
    {
        if (mapped[i] >= 'A' && mapped[i] <= 'Z')
        {
            mapped[i] = mapped[i] + ('a' - 'A');
        }
    }
    return nullptr;
}

void processFile(const string &fileName)
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

    size_t half_size = file_size / 2;
    ThreadData data1 = {mapped, 0, half_size};
    ThreadData data2 = {mapped, half_size, file_size - half_size};

    pthread_t thread1, thread2;
    if (pthread_create(&thread1, nullptr, convertToLowercase, &data1) != 0)
    {
        printf("Error creating thread 1\n");
        munmap(mapped, file_size);
        return;
    }

    if (pthread_create(&thread2, nullptr, convertToLowercase, &data2) != 0)
    {
        printf("Error creating thread 2\n");
        munmap(mapped, file_size);
        return;
    }

    pthread_join(thread1, nullptr);
    pthread_join(thread2, nullptr);

    char *data = static_cast<char *>(mapped);
    printf("Converted text:\n%.*s\n", (int)file_size, data);

    if (msync(mapped, file_size, MS_SYNC) == -1)
    {
        printf("Error syncing file changes\n");
    }

    if (munmap(mapped, file_size) == -1)
    {
        printf("Error unmapping file\n");
    }
}

int main(int argc, char *argv[])
{
    if (argc != 2)
    {
        printf("Pass filename as cmd arg");
        return 1;
    }

    string fileName = argv[1];
    processFile(fileName);

    return 0;
}
