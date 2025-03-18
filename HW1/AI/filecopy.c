// 3.27

#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

#define BUFFER_SIZE 1024

int main(int argc, char *argv[]) {
    if (argc != 3) {
        fprintf(stderr, "Usage: %s <source file> <destination file>\n",
                argv[0]);
        exit(EXIT_FAILURE);
    }

    int pipefd[2];
    pid_t pid;
    char buffer[BUFFER_SIZE];
    ssize_t bytesRead;

    if (pipe(pipefd) == -1) {
        perror("pipe");
        exit(EXIT_FAILURE);
    }

    pid = fork();
    if (pid == -1) {
        perror("fork");
        exit(EXIT_FAILURE);
    }

    if (pid == 0) {       // Child process
        close(pipefd[1]); // Close unused write end

        int destFile =
            open(argv[2], O_WRONLY | O_CREAT | O_TRUNC, S_IRUSR | S_IWUSR);
        if (destFile == -1) {
            perror("open destination file");
            exit(EXIT_FAILURE);
        }

        while ((bytesRead = read(pipefd[0], buffer, BUFFER_SIZE)) > 0) {
            if (write(destFile, buffer, bytesRead) != bytesRead) {
                perror("write to destination file");
                exit(EXIT_FAILURE);
            }
        }

        close(pipefd[0]);
        close(destFile);
    } else {              // Parent process
        close(pipefd[0]); // Close unused read end

        int srcFile = open(argv[1], O_RDONLY);
        if (srcFile == -1) {
            perror("open source file");
            exit(EXIT_FAILURE);
        }

        while ((bytesRead = read(srcFile, buffer, BUFFER_SIZE)) > 0) {
            if (write(pipefd[1], buffer, bytesRead) != bytesRead) {
                perror("write to pipe");
                exit(EXIT_FAILURE);
            }
        }

        close(pipefd[1]);
        close(srcFile);

        wait(NULL); // Wait for child process to finish
    }

    return 0;
}