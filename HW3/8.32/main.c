#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>

#define MAX_FARMERS 20
#define MAX_CROSSING_TIME 3

enum Direction { NORTH, SOUTH, NONE };
enum Direction current_direction = NONE;
int farmers_on_bridge = 0;

pthread_mutex_t bridge_mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t north_queue = PTHREAD_COND_INITIALIZER;
pthread_cond_t south_queue = PTHREAD_COND_INITIALIZER;

int waiting_north = 0;
int waiting_south = 0;

void cross_bridge(int farmer_id, enum Direction direction) {
    const char *dir_str = (direction == NORTH) ? "Northbound" : "Southbound";
    int crossing_time = (rand() % MAX_CROSSING_TIME) + 1;

    printf("Farmer %d (%s) is ON the bridge for %d seconds\n", farmer_id,
           dir_str, crossing_time);

    sleep(crossing_time);

    printf("Farmer %d (%s) has LEFT the bridge\n", farmer_id, dir_str);
}

void *farmer(void *arg) {
    int id = *(int *)arg;
    enum Direction direction = (id % 2 == 0) ? NORTH : SOUTH;
    const char *dir_str = (direction == NORTH) ? "Northbound" : "Southbound";

    printf("Farmer %d (%s) wants to cross the bridge\n", id, dir_str);

    pthread_mutex_lock(&bridge_mutex);

    if (direction == NORTH) {
        waiting_north++;
    } else {
        waiting_south++;
    }

    while ((current_direction != NONE && current_direction != direction) ||
           (direction == NORTH && waiting_south > 0 &&
            current_direction != NORTH) ||
           (direction == SOUTH && waiting_north > 0 &&
            current_direction != SOUTH)) {
        if (direction == NORTH) {
            pthread_cond_wait(&north_queue, &bridge_mutex);
        } else {
            pthread_cond_wait(&south_queue, &bridge_mutex);
        }
    }

    if (current_direction == NONE) {
        current_direction = direction;
    }

    farmers_on_bridge++;

    if (direction == NORTH) {
        waiting_north--;
    } else {
        waiting_south--;
    }

    pthread_mutex_unlock(&bridge_mutex);

    cross_bridge(id, direction);

    pthread_mutex_lock(&bridge_mutex);

    farmers_on_bridge--;

    if (farmers_on_bridge == 0) {
        current_direction = NONE;

        if (waiting_north > 0) {
            pthread_cond_broadcast(&north_queue);
        }
        if (waiting_south > 0) {
            pthread_cond_broadcast(&south_queue);
        }
    }

    pthread_mutex_unlock(&bridge_mutex);

    free(arg);
    return NULL;
}

int main() {
    srand(time(NULL));
    pthread_t threads[MAX_FARMERS];
    int i;

    printf("Bridge Crossing Simulation - One-Lane Bridge Problem\n\n");

    for (i = 0; i < MAX_FARMERS; i++) {
        int *id = malloc(sizeof(int));
        *id = i;
        pthread_create(&threads[i], NULL, farmer, id);
        usleep(rand() % 500000);
    }

    for (i = 0; i < MAX_FARMERS; i++) {
        pthread_join(threads[i], NULL);
    }

    pthread_mutex_destroy(&bridge_mutex);
    pthread_cond_destroy(&north_queue);
    pthread_cond_destroy(&south_queue);

    printf("\nAll farmers have crossed the bridge!\n");
    return 0;
}
