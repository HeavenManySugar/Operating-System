#include <limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define MAX_PAGES 10
#define MAX_REFERENCES 20

// Function to generate random page reference string
void generatePageReferenceString(int *references, int length) {
    static int seedSet = 0;
    if (!seedSet) {
        srand(time(NULL));
        seedSet = 1;
    }
    printf("Generated page reference string: ");
    for (int i = 0; i < length; i++) {
        references[i] = rand() % MAX_PAGES;
        printf("%d ", references[i]);
    }
    printf("\n\n");
}

// Function to check if a page is already in frames
int isPageInFrames(int *frames, int numFrames, int page) {
    for (int i = 0; i < numFrames; i++) {
        if (frames[i] == page) {
            return i; // Return the index where page is found
        }
    }
    return -1; // Page not found
}

// Function to print current frame status
void printFrames(int *frames, int numFrames, const char *algorithm) {
    printf("%s frames: ", algorithm);
    for (int i = 0; i < numFrames; i++) {
        if (frames[i] == -1) {
            printf("- ");
        } else {
            printf("%d ", frames[i]);
        }
    }
    printf("\n");
}

// FIFO Page Replacement Algorithm
int fifoPageReplacement(int *references, int numReferences, int numFrames) {
    int *frames = (int *)malloc(numFrames * sizeof(int));
    int pageFaults = 0;
    int nextFrameIndex = 0;

    // Initialize frames to -1 (empty)
    for (int i = 0; i < numFrames; i++) {
        frames[i] = -1;
    }

    printf("=== FIFO Page Replacement Algorithm ===\n");

    for (int i = 0; i < numReferences; i++) {
        int currentPage = references[i];
        printf("Reference %d: Page %d -> ", i + 1, currentPage);

        if (isPageInFrames(frames, numFrames, currentPage) == -1) {
            // Page fault occurred
            frames[nextFrameIndex] = currentPage;
            nextFrameIndex = (nextFrameIndex + 1) % numFrames;
            pageFaults++;
            printf("Page fault! ");
            printFrames(frames, numFrames, "");
        } else {
            printf("Page hit! ");
            printFrames(frames, numFrames, "");
        }
    }

    printf("Total page faults (FIFO): %d\n\n", pageFaults);
    free(frames);
    return pageFaults;
}

// LRU Page Replacement Algorithm
int lruPageReplacement(int *references, int numReferences, int numFrames) {
    int *frames = (int *)malloc(numFrames * sizeof(int));
    int *lastUsed = (int *)malloc(numFrames * sizeof(int));
    int pageFaults = 0;
    int time = 0;

    // Initialize frames and last used times
    for (int i = 0; i < numFrames; i++) {
        frames[i] = -1;
        lastUsed[i] = -1;
    }

    printf("=== LRU Page Replacement Algorithm ===\n");

    for (int i = 0; i < numReferences; i++) {
        int currentPage = references[i];
        printf("Reference %d: Page %d -> ", i + 1, currentPage);
        time++;

        int pageIndex = isPageInFrames(frames, numFrames, currentPage);

        if (pageIndex != -1) {
            // Page hit - update last used time
            lastUsed[pageIndex] = time;
            printf("Page hit! ");
            printFrames(frames, numFrames, "");
        } else {
            // Page fault occurred
            pageFaults++;

            // Find empty frame or least recently used frame
            int replaceIndex = -1;
            for (int j = 0; j < numFrames; j++) {
                if (frames[j] == -1) {
                    replaceIndex = j;
                    break;
                }
            }

            if (replaceIndex == -1) {
                // No empty frame, find LRU
                int minTime = lastUsed[0];
                replaceIndex = 0;
                for (int j = 1; j < numFrames; j++) {
                    if (lastUsed[j] < minTime) {
                        minTime = lastUsed[j];
                        replaceIndex = j;
                    }
                }
            }

            frames[replaceIndex] = currentPage;
            lastUsed[replaceIndex] = time;
            printf("Page fault! ");
            printFrames(frames, numFrames, "");
        }
    }

    printf("Total page faults (LRU): %d\n\n", pageFaults);
    free(frames);
    free(lastUsed);
    return pageFaults;
}

// Optimal Page Replacement Algorithm
int optimalPageReplacement(int *references, int numReferences, int numFrames) {
    int *frames = (int *)malloc(numFrames * sizeof(int));
    int pageFaults = 0;

    // Initialize frames to -1 (empty)
    for (int i = 0; i < numFrames; i++) {
        frames[i] = -1;
    }

    printf("=== Optimal Page Replacement Algorithm ===\n");

    for (int i = 0; i < numReferences; i++) {
        int currentPage = references[i];
        printf("Reference %d: Page %d -> ", i + 1, currentPage);

        if (isPageInFrames(frames, numFrames, currentPage) == -1) {
            // Page fault occurred
            pageFaults++;

            // Find empty frame
            int replaceIndex = -1;
            for (int j = 0; j < numFrames; j++) {
                if (frames[j] == -1) {
                    replaceIndex = j;
                    break;
                }
            }

            if (replaceIndex == -1) {
                // No empty frame, find optimal replacement
                int farthest = -1;
                replaceIndex = 0;

                for (int j = 0; j < numFrames; j++) {
                    int nextUse = INT_MAX;

                    // Find next use of this page
                    for (int k = i + 1; k < numReferences; k++) {
                        if (references[k] == frames[j]) {
                            nextUse = k;
                            break;
                        }
                    }

                    if (nextUse > farthest) {
                        farthest = nextUse;
                        replaceIndex = j;
                    }
                }
            }

            frames[replaceIndex] = currentPage;
            printf("Page fault! ");
            printFrames(frames, numFrames, "");
        } else {
            printf("Page hit! ");
            printFrames(frames, numFrames, "");
        }
    }

    printf("Total page faults (Optimal): %d\n\n", pageFaults);
    free(frames);
    return pageFaults;
}

int main(int argc, char *argv[]) {
    int numFrames;
    int references[MAX_REFERENCES];

    // Check command line arguments
    if (argc != 2) {
        printf("Usage: %s <number_of_frames>\n", argv[0]);
        printf("Example: %s 3\n", argv[0]);
        return 1;
    }

    numFrames = atoi(argv[1]);

    if (numFrames <= 0 || numFrames > MAX_PAGES) {
        printf("Error: Number of frames must be between 1 and %d\n", MAX_PAGES);
        return 1;
    }

    printf("Page Replacement Algorithms Simulation\n");
    printf("Number of frames: %d\n", numFrames);
    printf("Page numbers range: 0-%d\n", MAX_PAGES - 1);
    printf("Reference string length: %d\n\n", MAX_REFERENCES);
    fflush(stdout);

    // Generate random page reference string
    generatePageReferenceString(references, MAX_REFERENCES);

    // Apply each algorithm
    int fifoFaults = fifoPageReplacement(references, MAX_REFERENCES, numFrames);
    int lruFaults = lruPageReplacement(references, MAX_REFERENCES, numFrames);
    int optimalFaults =
        optimalPageReplacement(references, MAX_REFERENCES, numFrames);

    // Summary
    printf("=== SUMMARY ===\n");
    printf("FIFO Page Faults:    %d\n", fifoFaults);
    printf("LRU Page Faults:     %d\n", lruFaults);
    printf("Optimal Page Faults: %d\n", optimalFaults);
    printf("\nOptimal algorithm typically performs best, followed by LRU, then "
           "FIFO.\n");

    return 0;
}