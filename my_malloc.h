#ifndef MY_MALLOC_H
#define MY_MALLOC_H

#include <stddef.h> // For size_t

// Block metadata structure
typedef struct block_t {
    size_t size;
    int free;
    struct block_t *next;
    struct block_t *prev;
} block_t;

// Function prototypes
void *malloc(size_t size);
void free(void *ptr);
void *calloc(size_t num, size_t size);
void *realloc(void *ptr, size_t size);

void print_heap_bounds();
size_t calculate_memory_leaks();

#endif // MY_MALLOC_H