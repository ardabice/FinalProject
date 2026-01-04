#include <stdio.h>

#define DISK_SIZE 16

// 1. BITMAP ALLOCATION
int bitmap[DISK_SIZE] = {0};

int allocate_bitmap(int size) {
    for (int i = 0; i <= DISK_SIZE - size; i++) {
        int found = 1;
        for (int j = 0; j < size; j++) {
            if (bitmap[i + j] == 1) {
                found = 0;
                break;
            }
        }
        if (found) {
            for (int j = 0; j < size; j++) bitmap[i + j] = 1;
            return i;
        }
    }
    return -1;
}

void free_bitmap(int start, int size) {
    for (int i = start; i < start + size; i++) {
        bitmap[i] = 0;
    }
}

// 2. LINKED LIST ALLOCATION
struct Block {
    int id;
    int used;
    struct Block* next;
};

void allocate_linked(struct Block* head, int size) {
    struct Block* curr = head;
    while (curr != NULL) {
        if (!curr->used) {
            struct Block* temp = curr;
            int count = 0;
            while (temp != NULL && !temp->used && count < size) {
                temp = temp->next;
                count++;
            }
            if (count == size) {
                temp = curr;
                for (int i = 0; i < size; i++) {
                    temp->used = 1;
                    temp = temp->next;
                }
                return;
            }
        }
        curr = curr->next;
    }
}

int main() {
    printf("Bitmap and Linked List logic initialized.\n");
    return 0;
}