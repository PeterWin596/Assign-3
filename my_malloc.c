#include <unistd.h>
#include <stdint.h>
#include "my_malloc.h"
#include <stdio.h>
#include <string.h>

// Explicit declaration for sbrk
extern void *sbrk(int increment);

#define BLOCK_SIZE sizeof(block_t)

static block_t *heap_start = NULL; // Pointer to the first block

// Helper functions
block_t *find_best_fit(size_t size);
block_t *request_space(block_t *last, size_t size);
void split_block(block_t *block, size_t size);
block_t *get_block_ptr(void *ptr);

// Implement malloc with best-fit strategy
void *malloc(size_t size) {
    if (size <= 0) {
        return NULL;
    }

    block_t *block;
    if (!heap_start) { // First call
        block = request_space(NULL, size);
        if (!block) {
            return NULL;
        }
        heap_start = block;
    } else {
        block = find_best_fit(size); // Use best-fit instead of first-fit
        if (block) {
            // Found a suitable block
            if (block->size > size + BLOCK_SIZE) {
                split_block(block, size);
            }
            block->free = 0;
        } else {
            // No suitable block found, request more space
            block_t *last = heap_start;
            while (last->next) {
                last = last->next;
            }
            block = request_space(last, size);
            if (!block) {
                return NULL;
            }
        }
    }

    return (block + 1); // Return pointer to the data segment
}

// Implement the best-fit search
block_t *find_best_fit(size_t size) {
    block_t *current = heap_start;
    block_t *best_fit = NULL;

    while (current) {
        if (current->free && current->size >= size) {
            if (!best_fit || current->size < best_fit->size) {
                best_fit = current; // Update best-fit block
            }
        }
        current = current->next;
    }

    return best_fit; // Return the smallest suitable block
}

// Request more space from the OS
block_t *request_space(block_t *last, size_t size) {
    void *request = sbrk(size + BLOCK_SIZE); // Request memory from the OS
    if (request == (void *)-1) {
        return NULL; // sbrk failed
    }

    block_t *block = (block_t *)request; // Cast the request to a block_t pointer
    block->size = size;
    block->free = 0;
    block->next = NULL;
    block->prev = last;

    if (last) {
        last->next = block;
    }

    return block;
}

// Split a block into two if it's significantly larger than needed
void split_block(block_t *block, size_t size) {
    block_t *new_block = (block_t *)((char *)(block + 1) + size);
    new_block->size = block->size - size - BLOCK_SIZE;
    new_block->free = 1;
    new_block->next = block->next;
    new_block->prev = block;

    block->size = size;
    block->next = new_block;

    if (new_block->next) {
        new_block->next->prev = new_block;
    }
}

// Implement free
void free(void *ptr) {
    if (!ptr) {
        return;
    }

    block_t *block = get_block_ptr(ptr);
    block->free = 1;

    // Coalesce adjacent free blocks
    if (block->next && block->next->free) {
        block->size += BLOCK_SIZE + block->next->size;
        block->next = block->next->next;
        if (block->next) {
            block->next->prev = block;
        }
    }
    if (block->prev && block->prev->free) {
        block->prev->size += BLOCK_SIZE + block->size;
        block->prev->next = block->next;
        if (block->next) {
            block->next->prev = block->prev;
        }
    }
}

block_t *get_block_ptr(void *ptr) {
    return (block_t *)ptr - 1;
}

// Implement calloc
void *calloc(size_t num, size_t size) {
    size_t total_size = num * size;
    void *ptr = malloc(total_size);
    if (ptr) {
        memset(ptr, 0, total_size);
    }
    return ptr;
}

// Implement realloc
void *realloc(void *ptr, size_t size) {
    if (!ptr) {
        return malloc(size);
    }

    block_t *block = get_block_ptr(ptr);
    if (block->size >= size) {
        if (block->size > size + BLOCK_SIZE) {
            split_block(block, size);
        }
        return ptr;
    } else {
        void *new_ptr = malloc(size);
        if (!new_ptr) {
            return NULL;
        }
        memcpy(new_ptr, ptr, block->size);
        free(ptr);
        return new_ptr;
    }
}

// Print heap boundaries
void print_heap_bounds() {
    printf("Heap starts at: %p\n", (void *)heap_start);
    printf("Heap ends at: %p\n", (void *)sbrk(0));
}

// Calculate memory leaks
size_t calculate_memory_leaks() {
    size_t leaks = 0;
    block_t *current = heap_start;

    while (current) {
        if (!current->free) {
            leaks += current->size;
        }
        current = current->next;
    }

    return leaks;
}
