#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/wait.h>

int main(int argc, char *argv[])
{
    if (argc != 3)
    {
        printf("Error: No file names entered\n");
        return 1;
    }

    int pipe_fd[2];
    if (pipe(pipe_fd) == -1)
    {
        printf("Pipe creation failed\n");
        return 1;
    }

    int pid = fork();
    if (pid < 0)
    {
        printf("Fork failed\n");
        return 1;
    }

    if (pid > 0)
    {
        int src_fd = open(argv[1], O_RDONLY);
        if (src_fd < 0)
        {
            printf("Error opening source file\n");
            return 1;
        }

        close(pipe_fd[0]);

        char buffer[1024];
        int bytes_read;
        while ((bytes_read = read(src_fd, buffer, sizeof(buffer))) > 0)
        {
            write(pipe_fd[1], buffer, bytes_read);
        }

        close(src_fd);
        close(pipe_fd[1]);

        wait(NULL);
    }
    else
    {
        close(pipe_fd[1]);

        int dest_fd = open(argv[2], O_WRONLY | O_CREAT | O_TRUNC, 0644);
        if (dest_fd < 0)
        {
            printf("Error opening destination file\n");
            return 1;
        }

        dup2(pipe_fd[0], 0);
        close(pipe_fd[0]);

        dup2(dest_fd, 1);
        close(dest_fd);

        char buffer[1024];
        int bytes_read;
        while ((bytes_read = read(0, buffer, sizeof(buffer))) > 0)
        {
            write(1, buffer, bytes_read);
        }

        exit(0);
    }

    return 0;
}
