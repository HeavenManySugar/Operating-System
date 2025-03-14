#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

int main(int argc, char *argv[]) {
    if (argc != 2) {
        printf("Usage: %s <command>\n", argv[0]);
        return 1;
    }
    struct timeval start, end;

    gettimeofday(&start, NULL);

    pid_t pid = fork();
    if (pid < 0) {
        perror("fork");
        return 1;
    }
    if (pid == 0) {
        execlp(argv[1], argv[1], NULL);
        perror("execlp");
        return 1;
    }
    int status;
    waitpid(pid, &status, 0);

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