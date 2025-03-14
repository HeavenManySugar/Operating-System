// 3.19 Version 1

#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <string.h>

int main(int argc, char *argv[]) {
    if (argc < 2) {
        printf("Usage: %s <command> [args...]\n", argv[0]);
        return 1;
    }

    const char *shm_name = "/shm_time";
    int shm_fd = shm_open(shm_name, O_CREAT | O_RDWR, 0666);
    if (shm_fd == -1) {
        perror("shm_open");
        return 1;
    }

    if (ftruncate(shm_fd, sizeof(struct timeval)) == -1) {
        perror("ftruncate");
        return 1;
    }

    struct timeval *shared_time = mmap(NULL, sizeof(struct timeval), PROT_READ | PROT_WRITE, MAP_SHARED, shm_fd, 0);
    if (shared_time == MAP_FAILED) {
        perror("mmap");
        return 1;
    }

    struct timeval end;

    pid_t pid = fork();
    if (pid < 0) {
        perror("fork");
        return 1;
    }
    if (pid == 0) {
        gettimeofday(shared_time, NULL);
        execvp(argv[1], &argv[1]);
        perror("execvp");
        return 1;
    }

    int status;
    waitpid(pid, &status, 0);

    gettimeofday(&end, NULL);

    long seconds = end.tv_sec - shared_time->tv_sec;
    long microseconds = end.tv_usec - shared_time->tv_usec;
    if (microseconds < 0) {
        seconds--;
        microseconds += 1000000;
    }
    printf("Elapsed time: %ld.%06ld seconds\n", seconds, microseconds);

    shm_unlink(shm_name);

    return 0;
}