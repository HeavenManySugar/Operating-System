#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#define MAX_RESOURCES 5
// available_resources is the shared data involved in the race condition
int available_resources =
    MAX_RESOURCES; // (a) Data involved in the race condition

int decrease_count(int count) {
    // (b) Race condition can occur here: checking and updating
    // available_resources is not atomic
    if (available_resources < count)
        return -1;
    else {
        available_resources -=
            count; // (b) Race condition: modification of shared data
        return 0;
    }
}

int increase_count(int count) {
    available_resources +=
        count; // (b) Race condition: modification of shared data
    return 0;
}

// Function to simulate resource requests
void *resource_request(void *arg) {
    int count = *(int *)arg;
    if (decrease_count(count) == 0) {
        printf("Resources allocated: %d\n", count);
        sleep(1); // Simulate resource usage
        increase_count(count);
        printf("Resources released: %d\n", count);
    } else {
        printf("Request denied for: %d resources\n", count);
    }
    return NULL;
}

int main() {
    pthread_t threads[10];
    int requests[10] = {1, 2, 3, 4, 1, 2, 3, 1, 2, 1};

    for (int i = 0; i < 10; i++) {
        if (pthread_create(&threads[i], NULL, resource_request, &requests[i]) !=
            0) {
            perror("Failed to create thread");
            exit(1);
        }
    }

    for (int i = 0; i < 10; i++) {
        pthread_join(threads[i], NULL);
    }

    printf("Final available resources: %d\n", available_resources);
    return 0;
}