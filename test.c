#include <unistd.h>

void free(void *ptr) {
    if (ptr == NULL) {
        return;
    }

    char *block_start = (char *)ptr - sizeof(size_t);

    size_t block_size = *((size_t *)block_start);
    
    brk(block_start - block_size);
}