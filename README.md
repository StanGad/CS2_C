$${\color{red}CS-My malloc project-25/01}$$

## Contributors
##### - Stanislas GADECEAU
##### - Guillaume EGMANN
##### - Emma MOLERA
We realized this project as part of a week of computer science courses, we had to create our own versions of memory allocation functions malloc(3) , free(3), realloc(3), and calloc(3). 

## Overview 

The primary goal was to allocate memory on the program's heap. These custom functions follow the same prototype as the standard C library functions and are implemented using the brk(2) and sbrk(2) system calls.

The resulting implementation, named my_malloc, serves as a basic memory allocator. The allocator efficiently manages memory blocks using a linked list data structure, specifically a structure that we called Block. 

This structure contains essential information, such as the size of the block, its free status, and a pointer to the next block in the linked list.


## Features

- **Block Structure:** Defines a structure Block to represent memory blocks, including information about the block size, whether it is free, and a pointer to the next block.

- **Allocation:** Provides the mymalloc function to allocate memory for a given size.

- **Deallocation:** Implements the myfree function to release previously allocated memory.

- **Reallocation:** Supports the myrealloc function, allowing dynamic resizing of allocated memory.

- **Cleared Allocation:** Includes the mycalloc function, similar to calloc, for allocating and zero-initializing memory.



## Functions 

- **Place for block**

#### This function checks if there are existing free blocks with sufficient size to fulfill the allocation request. 

It iterates through the linked list of blocks and marks a suitable block as occupied (free = FALSE) if found.

- **Update list**

#### This function updates the linked list of blocks by adding a new block at the end. 

If first_block is NULL, it sets first_block to the new block. Otherwise, it traverses the list to find the last block and appends the new block.

- **My malloc**

#### This function allocates a block of memory of the specified size. 

It first checks if the size is zero, returning NULL if true. Then, it attempts to find a suitable free block using place_for_block(size). If no suitable block is found, it allocates a new block using sbrk and initializes the block's properties.

- **Merge free blocks**

#### This function combines adjacent free blocks in the linked list to optimize memory usage. 

It iterates through the list, merging consecutive free blocks by combining their sizes and updating the next block pointer.

- **Release memory**

#### This function releases memory back to the system using brk if a block is free. 

If the provided block is not NULL and is free, it moves the program break back to release the memory.

- **Myfree**

#### This function frees the memory block associated with the given pointer. 

It first checks if the pointer is NULL. Then, it casts the pointer to a Block and marks the block as free. It calls merge_free_blocks() to merge adjacent free blocks and releases memory using release_memory().

- **Myrealloc**

#### This function resizes the memory block pointed to by the given pointer to the specified size. 

If the pointer is NULL, it behaves like mymalloc(size). If the size is zero, it behaves like myfree(ptr). If the existing block is large enough, it returns the original pointer. Otherwise, it allocates a new block, copies data, and frees the old block.

- **Mycalloc**

#### This function allocates a block of memory for an array with elements initialized to zero. 

It calculates the total size, allocates memory using mymalloc(), and initializes the allocated memory to zero.

- **Main**

#### The main() function demonstrates the usage of our memory allocator by allocating and freeing memory for characters and integers. 

## Thank you and conclusion

We would like to thank Gregor for offering us this exercise that we have fun doing despite the many problems encountered.

This type of exercise is pleasant to learn new knowledge in C, project management, management of Github and the fact that we are free of our time is more challenging.

Thanks also to  [https://github.com/matthieu-sgi] @matthieu-sgi for his help and for his time.
