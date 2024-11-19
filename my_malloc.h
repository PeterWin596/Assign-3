#ifndef MY_MALLOC_H
#define MY_MALLOC_H

#include <stddef.h> // For size_t

// Block metadata structure
typedef struct block_t {
    size_t size;            // Size of the block
    int free;               // 1 if free, 0 if allocated
    struct block_t *next;   // Pointer to the next block
    struct block_t *prev;   // Pointer to the previous block
} block_t;

// Function prototypes
void *malloc(size_t size);
void free(void *ptr);
void *calloc(size_t num, size_t size);
void *realloc(void *ptr, size_t size);

void print_heap_bounds();
size_t calculate_memory_leaks();

#endif // MY_MALLOC_H
