// 3.19 Version 2

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/time.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

int main(int argc, char *argv[]) {
    if (argc < 2) {
        printf("Usage: %s <command> [args...]\n", argv[0]);
        return 1;
    }

    int pipefd[2];
    if (pipe(pipefd) == -1) {
        perror("pipe");
        return 1;
    }

    struct timeval start, end;

    pid_t pid = fork();
    if (pid < 0) {
        perror("fork");
        return 1;
    }
    if (pid == 0) {
        close(pipefd[0]);
        gettimeofday(&start, NULL);
        write(pipefd[1], &start, sizeof(start));
        close(pipefd[1]);
        execvp(argv[1], &argv[1]);
        perror("execlp");
        return 1;
    }

    close(pipefd[1]);
    waitpid(pid, NULL, 0);
    read(pipefd[0], &start, sizeof(start));
    close(pipefd[0]);

    gettimeofday(&end, NULL);

    long seconds = end.tv_sec - start.tv_sec;
    long microseconds = end.tv_usec - start.tv_usec;
    if (microseconds < 0) {
        seconds--;
        microseconds += 1000000;
    }
    printf("Elapsed time: %ld.%06ld seconds\n", seconds, microseconds);

    return 0;
}