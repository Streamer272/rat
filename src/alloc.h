#ifndef RAT_ALLOC_H
#define RAT_ALLOC_H

#include <stddef.h>

void *alloc(size_t size);
void *ralloc(void *ptr, size_t size, size_t add_size);

#endif
