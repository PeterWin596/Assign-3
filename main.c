#include "my_malloc.h"
#include <stdio.h>

int main() {
    print_heap_bounds();

    // Allocate memory blocks
    void *p1 = malloc(100);
    void *p2 = malloc(200);
    void *p3 = malloc(300); // Used later
    void *p4 = malloc(400);
    void *p5 = malloc(500); // Used later

    
    free(p2);
    free(p4);

    // Allocate a smaller block to test best-fit
    void *p6 = malloc(150);

    // Print allocations
    printf("Allocated 100 bytes at %p\n", p1);
    printf("Allocated 300 bytes at %p (unused)\n", p3); // Add a usage
    printf("Allocated 500 bytes at %p (unused)\n", p5); // Add a usage
    printf("Freed 200-byte block at %p\n", p2);
    printf("Allocated 150 bytes at %p\n", p6); // Should reuse freed block

    print_heap_bounds();

    // Calculate memory leaks
    printf("Memory leaks: %zu bytes\n", calculate_memory_leaks());
    return 0;
}
