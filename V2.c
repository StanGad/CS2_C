#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>

#define TRUE 1
#define FALSE 0

// Define a structure to represent memory blocks
typedef struct Block {
    size_t size;
    int free;
    struct Block* next;
} Block;

// Initialize the initial block
Block* first_block = NULL;

void place_for_block(size_t size) {
    // Check if there are any existing blocks that can fulfill the request
    Block* current_block = first_block;
    while (current_block != NULL) {
        if (current_block->free && current_block->size >= size) {
            current_block->free = FALSE;
            return;  // Block placed successfully
        }
        current_block = current_block->next;
    }
}

void update_list(Block* new_block) {
    if (first_block == NULL) {
        first_block = new_block;
    } else {
        Block* last_block = first_block;
        while (last_block->next != NULL) {
            last_block = last_block->next;
        }
        last_block->next = new_block;
    }
}

void* mymalloc(size_t size) {
    if (size == 0) {
        return NULL;
    }

    // Check if there is a suitable block in the existing free space
    place_for_block(size);

    // If no suitable block is found, allocate a new block using sbrk
    size_t block_size = sizeof(Block) + size;
    Block* current_block = sbrk(block_size);
    if (current_block == (void*) -1) {
        perror("sbrk");
        return NULL;
    }

    // Initialize the new block
    current_block->size = size;
    current_block->free = FALSE;
    current_block->next = NULL;

    // Update the linked list of blocks
    update_list(current_block);

    return (void*) (current_block + 1);  // Skip block information, returning pointer to actual data
}

void merge_free_blocks() {
    Block* current_block = first_block;
    while (current_block != NULL && current_block->next != NULL) {
        if (current_block->free && current_block->next->free) {
            // Merge adjacent free blocks
            current_block->size += current_block->next->size;
            current_block->next = current_block->next->next;
        } else {
            current_block = current_block->next;
        }
    }
}

void myfree(void* ptr) {
    if (ptr == NULL) {
        return;
    }

    // Cast the pointer to Block
    Block* current_block = (Block*)ptr - 1;

    if (!current_block->free) {
        // Mark the block as free
        current_block->free = TRUE;

        // Merge adjacent free blocks
        merge_free_blocks();
    }
}


int main() {
    char* a = (char*)mymalloc(sizeof(char));
    *a = 's';

    int* the_counter = (int*)mymalloc(sizeof(int));
    *the_counter = 0;

    printf("My char: %c\n", *a);

    myfree(a);
    myfree(the_counter);
    myfree(a);
    // Accessing a after freeing is undefined behavior
    // Avoid doing this in real code
    printf("My char: %c\n", *a);  // This is now vali

    return 0;
}
