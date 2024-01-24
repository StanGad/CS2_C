#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>

#define TRUE 1
#define FALSE 0

// Define a structure to represent memory blocks
typedef struct Block {
    size_t size;    
    int free;           
    struct Block* next;  //The link with the next block 
} Block;

// Initialize the initial block
Block* first_block = NULL;

void place_for_block(size_t size) {
    // Check if there are any existing blocks that can fulfill the request
    Block* current_block = first_block;
    while (current_block != NULL) {  //while the pointer address is not null
        if (current_block->free && current_block->size >= size) {  //If the block is free and the size of the free block is equal or superior compare to the data size
            current_block->free = FALSE; //The block is now occupied
            return;  // Block placed successfully
        }
        current_block = current_block->next; //It goes to the next block address 
    }
}



void update_list(Block* new_block) {
    if (first_block == NULL) { //If the pointer address of the first_block is null 
        first_block = new_block; //The first_block change with the new block
    } else {
        Block* last_block = first_block;  // In the other case, the pointer address of the last_block is equal to the pointer address of the first_block
        while (last_block->next != NULL) {  //While the link pointer address to the next block is not nut
            last_block = last_block->next; //The pointer address is equal to pointer addrress of the next block to see further if the pointer is NULL or Not
        }
        last_block->next = new_block; //The pointer address of the last block begin the new address of the next block
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

void release_memory(Block* block) {
    if (block != NULL && block->free) {
        // Move the program break back to release memory
        brk((void*)block);
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

        // Release memory using sbrk
        release_memory(current_block);
    }
}


void* myrealloc(void* ptr, size_t size) {
    if (ptr == NULL) {
        // If ptr is NULL, realloc behaves like malloc
        return mymalloc(size);
    }

    if (size == 0) {
        // If size is 0, realloc behaves like free
        myfree(ptr);
        return NULL;
    }

    // Cast the pointer to Block
    Block* current_block = (Block*)ptr - 1;

    if (current_block->size >= size) {
        // If the existing block is large enough, no need to allocate a new block
        return ptr;
    } else {
        // Allocate a new block and copy the data
        void* new_ptr = mymalloc(size);
        if (new_ptr == NULL) {
            return NULL; // Allocation failed
        }

        // Copy the data from the old block to the new block
        size_t min_size = (current_block->size < size) ? current_block->size : size;
        for (size_t i = 0; i < min_size; ++i) {
            ((char*)new_ptr)[i] = ((char*)ptr)[i];
        }

        // Free the old block
        myfree(ptr);

        return new_ptr;
    }
}

void* mycalloc(size_t num_elements, size_t element_size) {
    // Calculate the total size needed
    size_t total_size = num_elements * element_size;

    // Allocate memory using mymalloc
    void* ptr = mymalloc(total_size);

    if (ptr != NULL) {
        // Initialize the allocated memory to zero
        for (size_t i = 0; i < total_size; ++i) {
            ((char*)ptr)[i] = 0;
        }
    }

    return ptr;
}

int main() {
    char* a = (char*)mymalloc(sizeof(char));
    *a = 's';
    char* b = (char*)malloc(sizeof(char));
    *b ='h';

    int* the_counter = (int*)mymalloc(sizeof(int));
    *the_counter = 0;

    printf("My char: %c\n", *a);
    printf("My char: %c\n", *b);

    myfree(a);
    myfree(the_counter);
    myfree(b);

    // Accessing a after freeing is undefined behavior
    // Avoid doing this in real code
  //  a = (char*)mymalloc(sizeof(char));  // Allocate new memory if needed
    //printf("My char: %c\n", *a);  // This is now valid
    printf("My char: %c\n", *b);

    return 0;
}

