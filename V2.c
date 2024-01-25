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


// Give a block of memory in the heap to stock informations at a certain address 
void* mymalloc(size_t size) { 
    if (size == 0) {    //If the size entered in attribute is equal to 0, the function return NULL
        return NULL;
    }

    // Check if there is a suitable block in the existing free space
    place_for_block(size);

    // If no suitable block is found, allocate a new block using sbrk
    size_t block_size = sizeof(Block) + size; //Calcul the total size to allocate for the block. It's the sum of the struct block size and the size asked
    Block* current_block = sbrk(block_size); //Call sbrk() to allocate a new block of memory with the size of "size"
    if (current_block == (void*) -1) {  //
        perror("sbrk");  //Print the error "sbrk"
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
    Block* current_block = first_block;  // Initialize the address of the first block in the linked list 
    while (current_block != NULL && current_block->next != NULL) { //While the pointer address of the current block and the one of the next block are not null 
        if (current_block->free && current_block->next->free) { 
            // Merge adjacent free blocks
            current_block->size += current_block->next->size; //add the size of the next block to the current block, mixing both block in one bigger
            current_block->next = current_block->next->next; //update the pointer next of the current block to target the next block of the next block. And jumped the current block which has been merged with the next one
        } else {
            current_block = current_block->next;  //And if the blocks can't be merge together, the pointer is deplace to another block of the linked list
        }
    }
}

// Help to release the all memory in the free()
void release_memory(Block* block) {  
    if (block != NULL && block->free) { // if the pointer block is not NULL and the free is true
        // Move the program break back to release memory
        brk((void*)block);
    }
}

//Release a block of memory allocated dinamically in the heap
void myfree(void* ptr) {
    if (ptr == NULL) { //If the pointer is NULL ->return
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

// This function allow the reallocation of a memory block in the heap 
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


// This function allocate a block of memory initializing every octets with the value equals to 0
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
    *a = 'd';
   
    printf("1- Le pointeur est %p, est sa data est %c\n",a, *a);


    *a = 's';

    if(a !=NULL){
        printf("2- Le pointeur est %p, est sa data est %c\n",a, *a);
    }
    else{
        printf("the pointer is NULL");
    }
    


    
    

    int* b = (int*)mycalloc(5, sizeof(int));
    if (b != NULL){
        printf("3- Le pointeur est %p, est sa data est %i\n", b, *b);
        
    }
    else{
        printf("Huston on a un problème");
        printf("3- Le pointeur est %p, est sa data est %i\n", b, *b);
        return 1;
    }

    
    int* d = mymalloc(sizeof(int));
    int* c = myrealloc(d, sizeof(int)*20);

    if (c != NULL) {
        printf("4 - Le pointeur est %p, est sa data est %i\n", c, *c);
        
    }
    else{
        printf("error");
    }

    
    //THe memory has been realesed 
    myfree(a);
    if(a !=NULL){
        printf("5- Le pointeur est %p, est sa data est %c\n",a, *a);
    }
    else{
        printf("the memory has been realesed");
    }

    


    






    // Accessing a after freeing is undefined behavior
    // Avoid doing this in real code
    //a = (char*)mymalloc(sizeof(char));  // Allocate new memory if needed
    //printf("My char: %c\n", *a);  // This is now valid
    //printf("My char: %c\n", *b);

    return 0;
}

