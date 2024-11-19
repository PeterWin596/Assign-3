#include "my_malloc.h"
#include <stdio.h>

int main() {
    print_heap_bounds();

    void *ptrs[10];

    // Allocate memory
    for (int i = 0; i < 10; i++) {
        ptrs[i] = malloc((i + 1) * 100);
        printf("Allocated %d bytes at %p\n", (i + 1) * 100, ptrs[i]);
    }

    // Free even indexed pointers
    for (int i = 0; i < 10; i += 2) {
        free(ptrs[i]);
        printf("Freed memory at %p\n", ptrs[i]);
    }

    // Allocate more memory to test reuse
    void *new_ptr = malloc(250);
    printf("Allocated 250 bytes at %p\n", new_ptr);

    print_heap_bounds();

    // Calculate and print memory leaks
    size_t leaks = calculate_memory_leaks();
    printf("Memory leaks: %zu bytes\n", leaks);

    return 0;
}
