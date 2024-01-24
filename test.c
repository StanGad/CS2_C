#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>

void* mymalloc(size_t size){

    void* a = sbrk(size);


    if (size == 0){
        return;
    }


    if(a == (void *) -1 ){
        printf("FAILURE");
        return (void*)-1;
    }else{
        return a;
    }

}

void merge_free_block(){
    if (ptr==NULL){
        return;
    }

    while(ptr && ptr->next){
        if(ptr->free == TRUE && ptr->next->free == TRUE){
            ptr->size+=ptr->next->size;
            ptr->next=ptr->next->next;
            if(ptr->next != NULL){
                ptr->next->prev = ptr;
            }
            ptr = ptr->next;
        }
    }

}


void free(void *ptr) {
    if (ptr == NULL) {
        return;
    }
    
    ptr->free= TRUE;
    merge_free_block();
}

void main(){

    char* a = mymalloc(sizeof(char));
    *a = 's';

    int *the_counter = (int*) malloc(sizeof(int));
    *the_counter =0;
    printf("My char %c", *a);
}