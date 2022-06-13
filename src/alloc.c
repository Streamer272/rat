#include <malloc.h>
#include <stdlib.h>
#include <memory.h>
#include "alloc.h"

void *alloc(size_t size) {
    void *ptr = malloc(size);
    if (ptr == NULL) {
        perror("Couldn't allocate memory");
        exit(EXIT_FAILURE);
    }

    memset(ptr, 0, size);
    return ptr;
}

void *ralloc(void *ptr, size_t size) {
    void *new_ptr = realloc(ptr, size);
    if (new_ptr == NULL) {
        perror("Couldn't reallocate memory");
        exit(EXIT_FAILURE);
    }

    memset((void*) new_ptr + sizeof(ptr) - size, 0, size - sizeof(ptr));
    return new_ptr;
}
