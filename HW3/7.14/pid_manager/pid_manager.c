#include <pthread.h>
#include <stdlib.h>
#include <string.h>

#include "pid_manager.h"

#define MIN_PID 300
#define MAX_PID 5000

static int *pid_map = NULL;
static pthread_mutex_t pid_mutex = PTHREAD_MUTEX_INITIALIZER;

int allocate_map(void) {
    pid_map = (int *)malloc(sizeof(int) * (MAX_PID - MIN_PID + 1));
    if (pid_map == NULL) {
        return -1;
    }
    memset(pid_map, 0, sizeof(int) * (MAX_PID - MIN_PID + 1));
    return 1;
}

int allocate_pid(void) {
    int pid = -1;

    if (pid_map == NULL) {
        return -1;
    }

    pthread_mutex_lock(&pid_mutex);

    for (int i = 0; i < MAX_PID - MIN_PID + 1; i++) {
        if (pid_map[i] == 0) {
            pid_map[i] = 1;
            pid = i + MIN_PID;
            break;
        }
    }

    pthread_mutex_unlock(&pid_mutex);
    return pid;
}

void release_pid(int pid) {
    if (pid_map == NULL) {
        return;
    }

    if (pid < MIN_PID || pid > MAX_PID) {
        return;
    }

    pthread_mutex_lock(&pid_mutex);
    pid_map[pid - MIN_PID] = 0;
    pthread_mutex_unlock(&pid_mutex);
}