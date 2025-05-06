#include <pthread.h>
#include <semaphore.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#define BUFFER_SIZE 5
#define NUM_PRODUCERS 3
#define NUM_CONSUMERS 3
#define RUN_TIME 10

int buffer[BUFFER_SIZE];
int in = 0, out = 0;

sem_t buf_mutex, empty, full;
int running = 1;

void *producer(void *arg) {
    int id = *(int *)arg;
    while (running) {
        int item = rand() % 100;
        sem_wait(&empty);
        sem_wait(&buf_mutex);
        buffer[in] = item;
        printf("Producer %d produced: %d\n", id, item);
        in = (in + 1) % BUFFER_SIZE;
        sem_post(&buf_mutex);
        sem_post(&full);
        sleep(rand() % 2);
    }
    return NULL;
}

void *consumer(void *arg) {
    int id = *(int *)arg;
    while (running) {
        sem_wait(&full);
        sem_wait(&buf_mutex);
        int item = buffer[out];
        printf("Consumer %d consumed: %d\n", id, item);
        out = (out + 1) % BUFFER_SIZE;
        sem_post(&buf_mutex);
        sem_post(&empty);
        sleep(rand() % 2);
    }
    return NULL;
}

void stop_execution(int signum) {
    (void)signum; // Mark parameter as unused
    running = 0;
}

int main() {
    pthread_t prod_threads[NUM_PRODUCERS], cons_threads[NUM_CONSUMERS];
    int prod_ids[NUM_PRODUCERS], cons_ids[NUM_CONSUMERS];

    sem_init(&buf_mutex, 0, 1);
    sem_init(&empty, 0, BUFFER_SIZE);
    sem_init(&full, 0, 0);

    signal(SIGALRM, stop_execution);
    alarm(RUN_TIME);

    for (int i = 0; i < NUM_PRODUCERS; i++) {
        prod_ids[i] = i + 1;
        pthread_create(&prod_threads[i], NULL, producer, &prod_ids[i]);
    }

    for (int i = 0; i < NUM_CONSUMERS; i++) {
        cons_ids[i] = i + 1;
        pthread_create(&cons_threads[i], NULL, consumer, &cons_ids[i]);
    }

    for (int i = 0; i < NUM_PRODUCERS; i++) {
        pthread_join(prod_threads[i], NULL);
    }

    for (int i = 0; i < NUM_CONSUMERS; i++) {
        pthread_join(cons_threads[i], NULL);
    }

    sem_destroy(&buf_mutex);
    sem_destroy(&empty);
    sem_destroy(&full);

    printf("Execution stopped after %d seconds.\n", RUN_TIME);
    return 0;
}