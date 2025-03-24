// 3.19 Version 2 pipe

#include <stdio.h>
#include <sys/time.h>
#include <sys/wait.h>
#include <unistd.h>

int main(int argc, char **argv) {
    if (argc < 2) {
        printf("Usage: %s <command> [args...]\n", argv[0]);
        return 1;
    }
    int pipefd[2];
    if (pipe(pipefd) < 0) {
        perror("pipe");
        return 1;
    }

    pid_t pid = fork();
    if (pid < 0) {
        perror("fork");
        return 1;
    }
    if (pid == 0) {
        close(pipefd[0]);
        struct timeval start_time;
        gettimeofday(&start_time, NULL);
        write(pipefd[1], &start_time, sizeof(start_time));
        close(pipefd[1]);
        execvp(argv[1], &argv[1]);
        perror("execlp");
        return 1;
    }
    waitpid(pid, NULL, 0);
    struct timeval start_time, end_time;
    close(pipefd[1]);
    read(pipefd[0], &start_time, sizeof(start_time));
    close(pipefd[0]);
    gettimeofday(&end_time, NULL);
    printf("Elapsed time: %ld.%03ld seconds\n",
           end_time.tv_sec - start_time.tv_sec,
           (end_time.tv_usec - start_time.tv_usec) / 1000);
    return 0;
}