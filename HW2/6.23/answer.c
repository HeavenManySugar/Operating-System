#include <pthread.h>

#define MAX_RESOURCES 5
// available_resources is the shared data involved in the race condition
int available_resources =
    MAX_RESOURCES; // (a) Data involved in the race condition

pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;

int decrease_count(int count) {
    pthread_mutex_lock(&mutex);
    // (b) Race condition can occur here: checking and updating
    // available_resources is not atomic
    if (available_resources < count) {
        pthread_mutex_unlock(&mutex);
        return -1;
    } else {
        available_resources -=
            count; // (b) Race condition: modification of shared data
        pthread_mutex_unlock(&mutex);
        return 0;
    }
}

int increase_count(int count) {
    pthread_mutex_lock(&mutex);
    available_resources +=
        count; // (b) Race condition: modification of shared data
    pthread_mutex_unlock(&mutex);
    return 0;
}