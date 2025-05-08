#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

typedef struct {
    unsigned long long *fib_array;
    int count;
    int *next_index;
    pthread_mutex_t *mutex;
    pthread_cond_t *cond;
} fib_data_t;

void *fibonacci_thread(void *arg) {
    fib_data_t *data = (fib_data_t *)arg;
    unsigned long long *fib = data->fib_array;
    int n = data->count;

    if (n < 2) {
        return NULL;
    }

    for (int i = 2; i < n; ++i) {
        fib[i] = fib[i - 1] + fib[i - 2];
        pthread_mutex_lock(data->mutex);
        *(data->next_index) = i + 1;
        pthread_cond_signal(data->cond);
        pthread_mutex_unlock(data->mutex);
    }

    return NULL;
}

int main(int argc, char *argv[]) {
    if (argc != 2) {
        printf("Usage: %s <number of Fibonacci numbers>\n", argv[0]);
        return 1;
    }

    int n = atoi(argv[1]);
    if (n <= 1) {
        printf("Please enter a positive integer greater than 1.\n");
        return 1;
    }

    unsigned long long *fib_array = malloc(n * sizeof(unsigned long long));
    fib_array[0] = 0;
    fib_array[1] = 1;
    if (!fib_array) {
        perror("malloc");
        return 1;
    }

    pthread_mutex_t mutex;
    pthread_cond_t cond;
    pthread_mutex_init(&mutex, NULL);
    pthread_cond_init(&cond, NULL);

    int next_index = 2;
    fib_data_t data = {fib_array, n, &next_index, &mutex, &cond};

    pthread_t thread;
    pthread_create(&thread, NULL, fibonacci_thread, &data);

    printf("Fibonacci sequence as they are calculated:\n");
    int last_printed = -1;

    while (last_printed < n - 1) {
        pthread_mutex_lock(&mutex);
        while (next_index <= last_printed + 1) {
            pthread_cond_wait(&cond, &mutex);
        }

        for (int i = last_printed + 1; i < next_index; i++) {
            printf("%llu ", fib_array[i]);
            fflush(stdout);
        }

        last_printed = next_index - 1;
        pthread_mutex_unlock(&mutex);
    }

    printf("\n");
    pthread_join(thread, NULL);

    pthread_mutex_destroy(&mutex);
    pthread_cond_destroy(&cond);
    free(fib_array);

    return 0;
}