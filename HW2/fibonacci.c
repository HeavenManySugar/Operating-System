#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>

typedef struct {
    unsigned long long *fib_array;
    int count;
} fib_data_t;

void *fibonacci_thread(void *arg) {
    fib_data_t *data = (fib_data_t *)arg;
    unsigned long long *fib = data->fib_array;
    int n = data->count;
    if (n > 0)
        fib[0] = 0;
    if (n > 1)
        fib[1] = 1;
    for (int i = 2; i < n; ++i) {
        fib[i] = fib[i - 1] + fib[i - 2];
    }
    return NULL;
}

int main(int argc, char *argv[]) {
    if (argc != 2) {
        printf("Usage: %s <number of Fibonacci numbers>\n", argv[0]);
        return 1;
    }
    int n = atoi(argv[1]);
    if (n <= 0) {
        printf("Please enter a positive integer.\n");
        return 1;
    }
    unsigned long long *fib_array = malloc(n * sizeof(unsigned long long));
    if (!fib_array) {
        perror("malloc");
        return 1;
    }
    fib_data_t data = {fib_array, n};
    pthread_t thread;
    pthread_create(&thread, NULL, fibonacci_thread, &data);
    pthread_join(thread, NULL);

    printf("Fibonacci sequence:\n");
    for (int i = 0; i < n; ++i) {
        printf("%llu ", fib_array[i]);
    }
    printf("\n");
    free(fib_array);
    return 0;
}