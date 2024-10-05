#include <stdio.h>
#include <stdlib.h>
#include <time.h>

// constant to keep page size and number of frames
#define PAGE_SIZE 125
#define NUM_FRAMES 31

// Function to load pages into frames

void allocatePages(int numPages, int *frameMap, int framePool[]) {
    
    int allocatedPages = 0;  
    
    while (allocatedPages < numPages) {
        int randomFrame;
        int attempts = 0;  // Avoid infinite loops in case all frames are occupied


        do {
            randomFrame = rand() % NUM_FRAMES; 
            attempts++;
            if (attempts > NUM_FRAMES) {
                printf("Warning: The memory is full, there is no frame to hold page %d\n", allocatedPages);
                return;
            }
        } while (framePool[randomFrame] != -1);

        // Assign the page to the frame
        framePool[randomFrame] = allocatedPages;
        frameMap[allocatedPages] = randomFrame;

        printf("Page %d allocated to Frame %d\n", allocatedPages, randomFrame);
        allocatedPages++;
    }
}

// Function for address resolution
void resolveAddress(int taskSize, int numPages, int *frameMap, int logicAddr) {
    int pageIndex = logicAddr / PAGE_SIZE;
    int pageOffset = logicAddr % PAGE_SIZE;

    if (pageIndex >= numPages) {
        printf("Error: Logical address exceeds task size!\n");
        return;
    }
    int mappedFrame = frameMap[pageIndex];
    int physicalAddr = mappedFrame * PAGE_SIZE + pageOffset;

    printf("Logical Address %d -> Physical Address %d\n", logicAddr, physicalAddr);
}

int main() {
    srand(time(NULL));  // Seed random number generator

    int taskSize;
    int logicalAddr;

    // Taking input from the user
    printf("Enter the size of the task (in KB): ");
    scanf("%d", &taskSize);

    // Calculate number of pages and internal fragmentation
    int numPages = (taskSize + PAGE_SIZE - 1) / PAGE_SIZE;
    int fragmentation = (numPages * PAGE_SIZE) - taskSize;

    printf("\n=============================================\n");

    // Display details of the task
    printf("Task size: %dKB\n", taskSize);
    printf("Page size: %dKB\n", PAGE_SIZE);
    printf("Total pages: %d\n", numPages);

    if (fragmentation > 0) {
        printf("Internal fragmentation: %dKB\n", fragmentation);
    } else {
        printf("No internal fragmentation.\n");
    }

    // Initialize page table and memory frames
    int frameMap[numPages];  // Array to map pages to frames
    int framePool[NUM_FRAMES];  // Simulate available frames

    // Mark all frames as initially empty
    for (int i = 0; i < NUM_FRAMES; i++) {
        framePool[i] = -1;
    }

    // Load task's pages into random memory frames
    printf("\n=============================================\n");
    printf("Allocating pages to memory frames...\n");
    allocatePages(numPages, frameMap, framePool);

    // Ensure the output is flushed to the console
    printf("Hello world\n");
    fflush(stdout);  // Force the "Hello world" message to be printed before input

    // Request a logical address for address resolution
    printf("\nEnter a logical address to resolve (0 to %d): ", taskSize - 1);
    fflush(stdout);  // Flush the output buffer to ensure prompt visibility
    scanf("%d", &logicalAddr);

    resolveAddress(taskSize, numPages, frameMap, logicalAddr);

    return 0;
}
