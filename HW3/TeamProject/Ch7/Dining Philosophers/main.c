#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#define NUM_PHILOSOPHERS 5
#define THINKING 0
#define HUNGRY 1
#define EATING 2

pthread_mutex_t mutex;
pthread_cond_t cond[NUM_PHILOSOPHERS];
int state[NUM_PHILOSOPHERS];
const char *state_names[] = {"THINKING", "HUNGRY  ", "EATING  "};

void display_status() {
    pthread_mutex_lock(&mutex);
    printf("\n");
    printf("Current status:\n");
    for (int i = 0; i < NUM_PHILOSOPHERS; i++) {
        printf("Philosopher %d: %s ", i, state_names[state[i]]);
        if (state[i] == EATING)
            printf("🍽️ ");
        else if (state[i] == HUNGRY)
            printf("🙏 ");
        else
            printf("🤔 ");
        printf("\n");
    }
    printf("\n");
    pthread_mutex_unlock(&mutex);
}

void test(int philosopher) {
    if (state[philosopher] == HUNGRY &&
        state[(philosopher + 1) % NUM_PHILOSOPHERS] != EATING &&
        state[(philosopher + NUM_PHILOSOPHERS - 1) % NUM_PHILOSOPHERS] !=
            EATING) {
        state[philosopher] = EATING;
        pthread_cond_signal(&cond[philosopher]);
    }
}

void pickup(int philosopher) {
    pthread_mutex_lock(&mutex);
    state[philosopher] = HUNGRY;
    printf("Philosopher %d is hungry and trying to get forks.\n", philosopher);
    test(philosopher);
    while (state[philosopher] != EATING) {
        pthread_cond_wait(&cond[philosopher], &mutex);
    }
    pthread_mutex_unlock(&mutex);
}

void putdown(int philosopher) {
    pthread_mutex_lock(&mutex);
    state[philosopher] = THINKING;
    printf("Philosopher %d puts down forks.\n", philosopher);
    test((philosopher + 1) % NUM_PHILOSOPHERS);
    test((philosopher + NUM_PHILOSOPHERS - 1) % NUM_PHILOSOPHERS);
    pthread_mutex_unlock(&mutex);
}

void *philosopher(void *num) {
    int philosopher = *(int *)num;
    while (1) {
        printf("Philosopher %d is thinking.\n", philosopher);
        display_status();
        sleep(rand() % 3 + 1); // Thinking for 1-3 seconds

        pickup(philosopher);
        printf("Philosopher %d is eating.\n", philosopher);
        display_status();
        sleep(rand() % 2 + 1); // Eating for 1-2 seconds

        putdown(philosopher);
    }
}

int main() {
    pthread_t philosophers[NUM_PHILOSOPHERS];
    int philosopher_ids[NUM_PHILOSOPHERS];

    srand(time(NULL)); // Initialize random seed
    pthread_mutex_init(&mutex, NULL);
    for (int i = 0; i < NUM_PHILOSOPHERS; i++) {
        pthread_cond_init(&cond[i], NULL);
        philosopher_ids[i] = i;
        state[i] = THINKING;
    }

    for (int i = 0; i < NUM_PHILOSOPHERS; i++) {
        pthread_create(&philosophers[i], NULL, philosopher,
                       (void *)&philosopher_ids[i]);
    }

    for (int i = 0; i < NUM_PHILOSOPHERS; i++) {
        pthread_join(philosophers[i], NULL);
    }

    pthread_mutex_destroy(&mutex);
    for (int i = 0; i < NUM_PHILOSOPHERS; i++) {
        pthread_cond_destroy(&cond[i]);
    }

    return 0;
}