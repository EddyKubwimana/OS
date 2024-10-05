#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define PAGE_SIZE 578
#define NUM_FRAMES 15

// Function to simulate the loading of pages into memory frames
void allocatePages(int numPages, int *frameMap, int framePool[]) {
    int allocatedPages = 0;  // Track the number of pages allocated
    
    while (allocatedPages < numPages) {
        int randomFrame;
        int attempts = 0;  // Avoid infinite loops in case all frames are occupied

        // Try finding a free frame, if no free frames are left, warn and exit
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
        allocatedPages++;
    }

    // Print frame and page mapping as a table
    printf("\n===================================================================\n");

    printf("Memory Frame Allocation:\n\n");
    
    printf("\n===================================================================\n");

    // Print the frame numbers in the first row
    printf("Frames:   ");
    for (int i = 0; i < NUM_FRAMES; i++) {
        printf("%3d ", i);
    }
    printf("\n");

    // Print the page numbers (or empty slots) in the second row
    printf("Pages:    ");
    for (int i = 0; i < NUM_FRAMES; i++) {
        if (framePool[i] != -1) {
            printf("%3d ", framePool[i]);
        } else {
            printf("  - ");
        }
    }
    printf("\n===================================================================\n");
}

// Function for address resolution
void resolveAddress(int taskSize, int numPages, int *frameMap, int logicAddr) {
    int pageIndex = logicAddr / PAGE_SIZE;
    int displacement = logicAddr % PAGE_SIZE;

    if (pageIndex >= numPages) {
        printf("Warning: Logical address exceeds task size!\n");
        return;
    }
    int mappedFrame = frameMap[pageIndex];
    int physicalAddr = mappedFrame * PAGE_SIZE + displacement;

    printf("Logical Address %d -> Physical Address %d\n", logicAddr, physicalAddr);
     printf("\n===================================================================\n");
}

int main() {
    srand(time(NULL));  // Seed random number generator

    int taskSize;
    int logicalAddr;

    // Taking input from the user
    printf("Enter the size of the job (in KB): ");
    scanf("%d", &taskSize);

    // Calculate number of pages and internal fragmentation
    int numPages = (taskSize + PAGE_SIZE - 1) / PAGE_SIZE;
    int fragmentation = (numPages * PAGE_SIZE) - taskSize;

    printf("\n===================================================================\n");

    // Display details of the task
    printf("Job size: %dKB\n", taskSize);
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
     printf("\n===================================================================\n");

    printf("Allocating pages to memory frames...\n");
    allocatePages(numPages, frameMap, framePool);

    // Request a logical address for address resolution
    printf("\nEnter a logical address to resolve (0 to %d): ", taskSize - 1);
    fflush(stdout);  // Flush the output buffer to ensure prompt visibility
    scanf("%d", &logicalAddr);

    resolveAddress(taskSize, numPages, frameMap, logicalAddr);

    return 0;
}
