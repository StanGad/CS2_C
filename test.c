#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>

void* mymalloc(size_t size){

    void* a = sbrk(size);

    if(a == (void *) -1 ){
        printf("FAILURE");
        return (void*)-1;
    }else{
        return a;
    }

    

    // printf("pointer address %p", a);
    // a = sbrk(0);
    // printf("pointer address %p", a);

    // return EXIT_SUCCESS;
}


void free(void *ptr) {
    if (ptr == NULL) {
        return;
    }

    char *block_start = (char *)ptr - sizeof(size_t);

    size_t block_size = *((size_t *)block_start);
    
    brk(block_start - block_size);
}

void main(){

    char* a = mymalloc(sizeof(char));
    *a = 's';
    printf("My char %c", *a);
}