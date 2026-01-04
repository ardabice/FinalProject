#include <stdio.h>
#include <stdlib.h>
#include <time.h>

/* --- DATA STRUCTURES --- */

// Node structure for the free memory segments [cite: 86]
struct Node {
  int start;  // Starting index of the free block
  int length; // Size of the free block
  struct Node *next;
};

// Structure to track allocated blocks (required for deallocation simulation)
struct AllocatedNode {
  int start;
  int length;
  struct AllocatedNode *next;
};

struct Node *free_list = NULL;
struct AllocatedNode *alloc_list = NULL;
struct Node *last_ptr = NULL; // Pointer for Next Fit algorithm [cite: 93]

/* --- MEMORY MANAGEMENT FUNCTIONS --- */

// Initializes 100 units of simulated memory [cite: 86]
void init_memory() {
  // Clear existing lists if any
  while (free_list) {
    struct Node *t = free_list;
    free_list = free_list->next;
    free(t);
  }
  while (alloc_list) {
    struct AllocatedNode *t = alloc_list;
    alloc_list = alloc_list->next;
    free(t);
  }

  free_list = (struct Node *)malloc(sizeof(struct Node));
  free_list->start = 0;
  free_list->length = 100;
  free_list->next = NULL;
  last_ptr = free_list;
}

// Prints the current state of the free list [cite: 112]
void print_free_list() {
  struct Node *curr = free_list;
  if (!curr)
    printf("Memory fully occupied.");
  while (curr) {
    printf("[%d, %d] ", curr->start, curr->length);
    curr = curr->next;
  }
  printf("\n");
}

// Merges adjacent free blocks to prevent fragmentation [cite: 105]
void merge_blocks() {
  struct Node *curr = free_list;
  while (curr && curr->next) {
    if (curr->start + curr->length == curr->next->start) {
      struct Node *temp = curr->next;
      curr->length += temp->length;
      curr->next = temp->next;
      free(temp);
    } else {
      curr = curr->next;
    }
  }
}

// Splits a chosen free block into allocated and remaining parts [cite: 103]
void split_block(struct Node *prev, struct Node *target, int size) {
  // Record for deallocation tracing
  struct AllocatedNode *a =
      (struct AllocatedNode *)malloc(sizeof(struct AllocatedNode));
  a->start = target->start;
  a->length = size;
  a->next = alloc_list;
  alloc_list = a;

  if (target->length == size) {
    // Perfect fit: remove block from free list
    if (prev == NULL)
      free_list = target->next;
    else
      prev->next = target->next;
    if (last_ptr == target)
      last_ptr = free_list;
    free(target);
  } else {
    // Partial fit: adjust block boundaries
    target->start += size;
    target->length -= size;
  }
}

/* --- ALLOCATION ALGORITHMS (Question 3.B) --- */

// BEST FIT: Scans the whole list for the smallest sufficient block [cite: 91]
int allocate_best_fit(int size) {
  struct Node *curr = free_list, *prev = NULL;
  struct Node *best = NULL, *best_p = NULL;
  int min_waste = 101;

  while (curr) {
    if (curr->length >= size && (curr->length - size) < min_waste) {
      min_waste = curr->length - size;
      best = curr;
      best_p = prev;
    }
    prev = curr;
    curr = curr->next;
  }
  if (best) {
    split_block(best_p, best, size);
    return 1;
  }
  return 0; // Allocation failed
}

// WORST FIT: Scans the whole list for the largest available block [cite: 92]
int allocate_worst_fit(int size) {
  struct Node *curr = free_list, *prev = NULL;
  struct Node *worst = NULL, *worst_p = NULL;
  int max_len = -1;

  while (curr) {
    if (curr->length >= size && curr->length > max_len) {
      max_len = curr->length;
      worst = curr;
      worst_p = prev;
    }
    prev = curr;
    curr = curr->next;
  }
  if (worst) {
    split_block(worst_p, worst, size);
    return 1;
  }
  return 0;
}

// NEXT FIT: Starts searching from the last successful allocation point [cite:
// 93]
int allocate_next_fit(int size) {
  struct Node *curr = last_ptr ? last_ptr : free_list;
  struct Node *start_point = curr;

  do {
    if (curr && curr->length >= size) {
      struct Node *p = free_list;
      struct Node *prev = NULL;
      while (p != curr) {
        prev = p;
        p = p->next;
      }
      last_ptr = curr->next;
      split_block(prev, curr, size);
      return 1;
    }
    curr = (curr && curr->next) ? curr->next : free_list;
  } while (curr != start_point);
  return 0;
}

// FREE: Returns a block to the free list and triggers merge [cite: 100, 105]
void free_memory(int size) {
  struct AllocatedNode *curr_a = alloc_list, *prev_a = NULL;
  int start_addr = -1;

  // Find the first allocated block of the specified size for simulation [cite:
  // 110]
  while (curr_a) {
    if (curr_a->length == size) {
      start_addr = curr_a->start;
      if (prev_a)
        prev_a->next = curr_a->next;
      else
        alloc_list = curr_a->next;
      free(curr_a);
      break;
    }
    prev_a = curr_a;
    curr_a = curr_a->next;
  }

  if (start_addr == -1)
    return;

  struct Node *new_node = (struct Node *)malloc(sizeof(struct Node));
  new_node->start = start_addr;
  new_node->length = size;

  // Ordered insertion into free list
  if (!free_list || start_addr < free_list->start) {
    new_node->next = free_list;
    free_list = new_node;
  } else {
    struct Node *temp = free_list;
    while (temp->next && temp->next->start < start_addr)
      temp = temp->next;
    new_node->next = temp->next;
    temp->next = new_node;
  }
  merge_blocks();
}

/* --- EXPERIMENTS (Question 3.C, 3.D & 3.E) --- */

// Experiment 1: Allocation Trace [cite: 107]
void run_experiment_1_trace() {
  int algorithms[] = {1, 2, 3}; // 1: Best, 2: Worst, 3: Next
  char *names[] = {"BEST FIT", "WORST FIT", "NEXT FIT"};
  int trace[] = {10, 5, 20, -5, 12, -10, 8, 6, 7, 3, 10}; // [cite: 108]

  for (int a = 0; a < 3; a++) {
    init_memory();
    printf("\n--- Experiment 1: %s Trace ---\n", names[a]);
    for (int i = 0; i < 11; i++) {
      if (trace[i] > 0) {
        if (algorithms[a] == 1)
          allocate_best_fit(trace[i]);
        else if (algorithms[a] == 2)
          allocate_worst_fit(trace[i]);
        else
          allocate_next_fit(trace[i]);
      } else {
        free_memory(-trace[i]); // [cite: 110]
      }
      printf("Step %d (%d): ", i + 1, trace[i]);
      print_free_list(); //
    }
  }
}

// Experiment 2: Fragmentation Test [cite: 120]
void run_experiment_2_fragmentation() {
  int algorithms[] = {1, 2, 3};
  char *names[] = {"BEST FIT", "WORST FIT", "NEXT FIT"};

  printf("\n--- Experiment 2: Fragmentation Test ---\n");
  for (int a = 0; a < 3; a++) {
    init_memory();
    // 1. Perform 12 random allocations (sizes 3-12) [cite: 122]
    for (int i = 0; i < 12; i++) {
      int size = (rand() % 10) + 3;
      if (a == 0)
        allocate_best_fit(size);
      else if (a == 1)
        allocate_worst_fit(size);
      else
        allocate_next_fit(size);
    }
    // 2. Free exactly 4 previously allocated blocks at random [cite: 123]
    for (int i = 0; i < 4; i++) {
      if (alloc_list != NULL)
        free_memory(alloc_list->length);
    }

    printf("%s Final List before 25-unit request: ", names[a]);
    print_free_list();

    // 3. Attempt one large allocation of size 25 [cite: 124]
    int success = 0;
    if (a == 0)
      success = allocate_best_fit(25);
    else if (a == 1)
      success = allocate_worst_fit(25);
    else
      success = allocate_next_fit(25);

    printf("Result for 25-unit allocation: %s\n\n",
           success ? "SUCCESS" : "FAILED");
  }
}

// Experiment 3: Speed Test [cite: 129]
void run_experiment_3_speed() {
  clock_t start, end;
  double cpu_time_used;
  char *names[] = {"BEST FIT", "WORST FIT", "NEXT FIT"};

  printf("--- Experiment 3: Speed Test (200 Cycles) ---\n");
  for (int a = 0; a < 3; a++) {
    init_memory();
    start = clock();
    for (int i = 0; i < 200; i++) { // [cite: 130]
      int size = (rand() % 10) + 1; // [cite: 131]
      if (a == 0)
        allocate_best_fit(size);
      else if (a == 1)
        allocate_worst_fit(size);
      else
        allocate_next_fit(size);

      if (alloc_list != NULL)
        free_memory(alloc_list->length); // [cite: 132]
    }
    end = clock();
    cpu_time_used = ((double)(end - start)) / CLOCKS_PER_SEC;
    printf("%s Total Time: %f seconds\n", names[a],
           cpu_time_used); // [cite: 133]
  }
}

int main() {
  srand(time(NULL)); // Random seed for experiments

  // Execute all experiments in order
  run_experiment_1_trace();         // Trace Analysis [cite: 107]
  run_experiment_2_fragmentation(); // Fragmentation Test [cite: 120]
  run_experiment_3_speed();         // Speed Test [cite: 129]

  return 0;
}