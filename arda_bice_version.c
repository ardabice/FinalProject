#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define MAX_DISK 32 

// Global bitmap array
int is_full[MAX_DISK]; 

// Linked List structure for Question 2 requirements
struct DiskBlock {
    int id;
    int busy;
    struct DiskBlock* next;
};
struct DiskBlock* head_node = NULL;

// Helper to reset the disk state
void reset_disk() {
    for(int i = 0; i < MAX_DISK; i++) {
        is_full[i] = 0;
    }
}

// Bitmap Allocation Function (Question 2.A)
int find_space(int how_many) {
    for (int i = 0; i <= MAX_DISK - how_many; i++) {
        int check = 1;
        for (int j = 0; j < how_many; j++) {
            if (is_full[i + j] == 1) { 
                check = 0; 
                break; 
            }
        }
        if (check == 1) {
            for (int j = 0; j < how_many; j++) is_full[i + j] = 1;
            return i; // Success: returns start index
        }
    }
    return -1; // Failure
}

// --- EXPERIMENTS (Question 2.B) ---

void run_experiments() {
    // 1. Speed Test (100 allocations)
    printf("\n--- Experiment 1: Speed Test (100 Runs) ---\n");
    clock_t start = clock();
    for(int i = 0; i < 100; i++) {
        find_space(1);
    }
    clock_t end = clock();
    double time_spent = (double)(end - start) / CLOCKS_PER_SEC;
    printf("Total Time for 100 allocations: %f seconds\n", time_spent);

    // 2. Fragmentation Test
    printf("\n--- Experiment 2: Fragmentation Test ---\n");
    reset_disk();
    // Fill with random small files
    for(int i = 0; i < 20; i++) {
        find_free_space: 
        if(find_space((rand() % 3) + 1) == -1) break;
    }
    // Create holes to simulate fragmentation
    for(int i = 0; i < MAX_DISK; i += 4) is_full[i] = 0; 
    
    printf("Disk state: ");
    for(int i = 0; i < MAX_DISK; i++) printf("%d", is_full[i]);
    
    int res = find_space(12); // Try to fit a large block (size 12)
    printf("\nRequest 12 units: %s\n", (res != -1) ? "SUCCESS" : "FAILED");

    // 3. Allocation Trace (The specific sequence from Question 2.B.3)
    printf("\n--- Experiment 3: Allocation Trace (First 15) ---\n");
    reset_disk();
    int trace[] = {2, 3, 5, 2, 4, 6, 1, 3, 5, 2, 4, 3, 2, 1, 5};
    for(int i = 0; i < 15; i++) {
        find_space(trace[i]);
        printf("Step %d (Size %d): ", i+1, trace[i]);
        for(int j = 0; j < MAX_DISK; j++) {
            printf("%d", is_full[j]);
        }
        printf("\n");
    }
}

int main() {
    srand(time(NULL));
    run_experiments();
    return 0;
}