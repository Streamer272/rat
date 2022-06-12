#include <malloc.h>
#include <stdlib.h>
#include "alloc.h"

void *alloc(size_t size) {
    void *ptr = malloc(size);
    if (ptr == NULL) {
        perror("Couldn't allocate memory");
        exit(EXIT_FAILURE);
    }
    return ptr;
}
