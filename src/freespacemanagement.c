#include <stdio.h>
#include <stdlib.h>

// FreeBlock structure - represents a free block of memory
typedef struct FreeBlock {
    int size;
    struct FreeBlock* next;
} FreeBlock;

// FreeList structure - manages the list of free blocks
typedef struct {
    FreeBlock* head;
} FreeList;

// Insert a free block at the end of the list
void insertBlock(FreeList* list, int size) {
    FreeBlock* newBlock = (FreeBlock*)malloc(sizeof(FreeBlock));
    FreeBlock* temp = list->head;
    newBlock->size = size;
    newBlock->next = NULL;

    if (list->head == NULL) {
        list->head = newBlock;
    } else {
        while (temp->next != NULL) {
            temp = temp->next;
        }
        temp->next = newBlock;
    }
}

// Print the free list
void printFreeList(FreeList* list) {
    FreeBlock* temp = list->head;
    printf("Free List: ");
    while (temp != NULL) {
        printf("[%d] -> ", temp->size);
        temp = temp->next;
    }
    printf("NULL\n");
}

// Allocate memory - find a block big enough and remove/shrink it
void allocate(FreeList* list, int requestSize) {
    FreeBlock *temp = list->head, *prev = NULL;

    while (temp != NULL) {
        if (temp->size >= requestSize) {
            printf("Allocating %d bytes from block of size %d\n", requestSize, temp->size);

            if (temp->size == requestSize) {
                // Exact fit - remove the block entirely
                if (prev == NULL) {
                    list->head = temp->next;
                } else {
                    prev->next = temp->next;
                }
                free(temp);
            } else {
                // Shrink the block by the requested amount
                temp->size -= requestSize;
            }
            return;
        }
        prev = temp;
        temp = temp->next;
    }

    printf("Allocation of %d bytes failed: no block large enough\n", requestSize);
}

// Deallocate memory - add a freed block back to the list
void deallocate(FreeList* list, int size) {
    printf("Deallocating %d bytes\n", size);
    insertBlock(list, size);
}

int main() {
    FreeList list;
    list.head = NULL;

    // Initialize free memory blocks
    insertBlock(&list, 200);
    insertBlock(&list, 500);
    insertBlock(&list, 100);

    printf("Initial ");
    printFreeList(&list);

    // Allocate some memory
    allocate(&list, 150);
    printf("After allocation: ");
    printFreeList(&list);

    // Allocate exact fit
    allocate(&list, 100);
    printf("After allocation: ");
    printFreeList(&list);

    // Try to allocate too much
    allocate(&list, 600);
    printf("After failed allocation: ");
    printFreeList(&list);

    // Deallocate (free) some memory
    deallocate(&list, 300);
    printf("After deallocation: ");
    printFreeList(&list);

    return 0;
}