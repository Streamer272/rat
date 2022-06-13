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

void *ralloc(void *ptr, size_t size, size_t add_size) {
    void *new_ptr = malloc(size + add_size);
    if (new_ptr == NULL) {
        perror("Couldn't reallocate memory");
        exit(EXIT_FAILURE);
    }
    for (int i = 0; i < size; i++) {
        ((char *) new_ptr)[i] = ((char *) ptr)[i];
    }

    memset((void*) new_ptr + size, 0, add_size);
    return new_ptr;
}
