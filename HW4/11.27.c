#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define CYLINDERS 5000
#define REQUESTS 1000

// Function prototypes
void generate_requests(int requests[], int num_requests);
int fcfs(int requests[], int num_requests, int initial_head);
int scan(int requests[], int num_requests, int initial_head);
int cscan(int requests[], int num_requests, int initial_head);
int compare(const void *a, const void *b);

int main(int argc, char *argv[]) {
    if (argc != 2) {
        printf("Usage: %s <initial_head_position>\n", argv[0]);
        printf("Initial head position should be between 0 and %d\n",
               CYLINDERS - 1);
        return 1;
    }

    int initial_head = atoi(argv[1]);

    // Validate initial head position
    if (initial_head < 0 || initial_head >= CYLINDERS) {
        printf("Error: Initial head position must be between 0 and %d\n",
               CYLINDERS - 1);
        return 1;
    }

    // Seed random number generator
    srand(time(NULL));

    // Generate random requests
    int requests[REQUESTS];
    generate_requests(requests, REQUESTS);

    printf("Disk Scheduling Simulation\n");
    printf("==========================\n");
    printf("Number of cylinders: %d (0 to %d)\n", CYLINDERS, CYLINDERS - 1);
    printf("Number of requests: %d\n", REQUESTS);
    printf("Initial head position: %d\n\n", initial_head);

    // Display first 10 requests as sample
    printf("First 10 requests: ");
    for (int i = 0; i < 10; i++) {
        printf("%d ", requests[i]);
    }
    printf("...\n\n");

    // Run FCFS algorithm
    int fcfs_movement = fcfs(requests, REQUESTS, initial_head);

    // Run SCAN algorithm
    int scan_movement = scan(requests, REQUESTS, initial_head);

    // Run C-SCAN algorithm
    int cscan_movement = cscan(requests, REQUESTS, initial_head);

    // Display results
    printf("Results:\n");
    printf("--------\n");
    printf("FCFS total head movement:   %d cylinders\n", fcfs_movement);
    printf("SCAN total head movement:   %d cylinders\n", scan_movement);
    printf("C-SCAN total head movement: %d cylinders\n", cscan_movement);

    printf("\nPerformance comparison:\n");
    printf("Best performing algorithm: ");
    if (fcfs_movement <= scan_movement && fcfs_movement <= cscan_movement) {
        printf("FCFS\n");
    } else if (scan_movement <= cscan_movement) {
        printf("SCAN\n");
    } else {
        printf("C-SCAN\n");
    }

    return 0;
}

// Generate random cylinder requests
void generate_requests(int requests[], int num_requests) {
    for (int i = 0; i < num_requests; i++) {
        requests[i] = rand() % CYLINDERS;
    }
}

// FCFS (First-Come, First-Served) algorithm
int fcfs(int requests[], int num_requests, int initial_head) {
    int total_movement = 0;
    int current_head = initial_head;

    printf("Running FCFS algorithm...\n");

    for (int i = 0; i < num_requests; i++) {
        int movement = abs(requests[i] - current_head);
        total_movement += movement;
        current_head = requests[i];
    }

    return total_movement;
}

// SCAN algorithm (Elevator algorithm)
int scan(int requests[], int num_requests, int initial_head) {
    int total_movement = 0;
    int current_head = initial_head;

    printf("Running SCAN algorithm...\n");

    // Create a copy of requests to sort
    int sorted_requests[REQUESTS];
    memcpy(sorted_requests, requests, sizeof(int) * num_requests);

    // Sort the requests
    qsort(sorted_requests, num_requests, sizeof(int), compare);

    // Find the position where we should start scanning
    int start_pos = 0;
    for (int i = 0; i < num_requests; i++) {
        if (sorted_requests[i] >= current_head) {
            start_pos = i;
            break;
        }
    }

    // Service requests from current position to the end (moving right)
    for (int i = start_pos; i < num_requests; i++) {
        total_movement += abs(sorted_requests[i] - current_head);
        current_head = sorted_requests[i];
    }

    // Move to the end of disk if we haven't reached it
    if (current_head < CYLINDERS - 1) {
        total_movement += (CYLINDERS - 1) - current_head;
        current_head = CYLINDERS - 1;
    }

    // Service remaining requests (moving left)
    for (int i = start_pos - 1; i >= 0; i--) {
        total_movement += abs(sorted_requests[i] - current_head);
        current_head = sorted_requests[i];
    }

    return total_movement;
}

// C-SCAN algorithm (Circular SCAN)
int cscan(int requests[], int num_requests, int initial_head) {
    int total_movement = 0;
    int current_head = initial_head;

    printf("Running C-SCAN algorithm...\n");

    // Create a copy of requests to sort
    int sorted_requests[REQUESTS];
    memcpy(sorted_requests, requests, sizeof(int) * num_requests);

    // Sort the requests
    qsort(sorted_requests, num_requests, sizeof(int), compare);

    // Find the position where we should start scanning
    int start_pos = 0;
    for (int i = 0; i < num_requests; i++) {
        if (sorted_requests[i] >= current_head) {
            start_pos = i;
            break;
        }
    }

    // Service requests from current position to the end (moving right)
    for (int i = start_pos; i < num_requests; i++) {
        total_movement += abs(sorted_requests[i] - current_head);
        current_head = sorted_requests[i];
    }

    // Move to the end of disk if we haven't reached it
    if (current_head < CYLINDERS - 1) {
        total_movement += (CYLINDERS - 1) - current_head;
        current_head = CYLINDERS - 1;
    }

    // Jump to the beginning of disk (circular)
    total_movement += current_head; // Move from end to beginning
    current_head = 0;

    // Service remaining requests from beginning
    for (int i = 0; i < start_pos; i++) {
        total_movement += abs(sorted_requests[i] - current_head);
        current_head = sorted_requests[i];
    }

    return total_movement;
}

// Comparison function for qsort
int compare(const void *a, const void *b) { return (*(int *)a - *(int *)b); }