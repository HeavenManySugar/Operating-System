// 3.19 Version 1 shared memory

#include <fcntl.h>
#include <stdint.h>
#include <stdio.h>
#include <sys/mman.h>
#include <sys/wait.h>
#include <time.h>
#include <unistd.h>

int main(int argc, char **argv) {
    if (argc < 2) {
        printf("Usage: %s <command> [args...]\n", argv[0]);
        return 1;
    }
    int start_time_fd = shm_open("start_time", O_CREAT | O_RDWR, 0666);
    ftruncate(start_time_fd, sizeof(struct timeval));
    struct timeval *start_time = (struct timeval *)mmap(
        NULL, sizeof(struct timeval), PROT_READ | PROT_WRITE, MAP_SHARED,
        start_time_fd, 0);
    pid_t pid = fork();
    if (pid < 0) {
        perror("fork");
        return 1;
    }
    if (pid == 0) {
        gettimeofday(start_time, NULL);
        execvp(argv[1], &argv[1]);
        perror("execlp");
        return 1;
    }
    waitpid(pid, NULL, 0);
    struct timeval end_time;
    gettimeofday(&end_time, NULL);
    printf("Elapsed time: %ld.%03ld seconds\n",
           end_time.tv_sec - start_time->tv_sec,
           (end_time.tv_usec - start_time->tv_usec) / 1000);
    shm_unlink("start_time");
    return 0;
}