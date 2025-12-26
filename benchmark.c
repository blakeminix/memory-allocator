#include "mini_malloc.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define OPS 200000

double time_alloc_mini() {
    clock_t start = clock();

    for (int i = 0; i < OPS; i++) {
        void* p = mini_malloc(32);
        mini_free(p);
    }

    return (double) (clock() - start) / CLOCKS_PER_SEC;
}

double time_alloc_std() {
    clock_t start = clock();

    for (int i = 0; i < OPS; i++) {
        void* p = malloc(32);
        free(p);
    }

    return (double) (clock() - start) / CLOCKS_PER_SEC;
}

int main() {
    mini_heap_init(512 * 1024);

    double t_mini = time_alloc_mini();
    double t_std  = time_alloc_std();

    printf("\nBenchmark Results (average time for allocation and free)\n");
    printf("Allocator     | Total Time (s) | Average Time (ns)\n");
    printf("mini_malloc   | %14.6f | %20.2f\n", t_mini, (t_mini / OPS) * 1e9 * 1e3);
    printf("malloc (std)  | %14.6f | %20.2f\n", t_std,  (t_std / OPS)  * 1e9 * 1e3);

    if (t_mini < t_std) {
        printf("\nmini_malloc is faster than malloc for 32-byte ops.\n");
    } else {
        printf("\nmalloc is faster than mini_malloc for 32-byte ops.\n");
    }

    return 0;
}