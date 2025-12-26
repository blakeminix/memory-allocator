#include "mini_malloc.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define NUM_BLOCKS 8000

int main() {
    mini_heap_init(512 * 1024);
    srand(time(NULL));

    void* blocks[NUM_BLOCKS];

    printf("\nAllocating small blocks\n");
    for (int i = 0; i < NUM_BLOCKS; i++) {
        blocks[i] = mini_malloc(32);
    }
    mini_heap_stats();

    printf("\nFreeing 50%% of them randomly\n");
    for (int i = 0; i < NUM_BLOCKS; i++) {
        if (rand() % 2 == 0) {
            mini_free(blocks[i]);
            blocks[i] = NULL;
        }
    }
    mini_heap_stats();

    printf("\nAllocating medium blocks after fragmentation exists\n");
    void* medium_blocks[200];

    for (int i = 0; i < 200; i++) {
        medium_blocks[i] = mini_malloc(256);
    }
    mini_heap_stats();

    printf("\nFreeing everything\n");
    for (int i = 0; i < 200; i++) {
        if (medium_blocks[i]) {
            mini_free(medium_blocks[i]);
        }
    }

    for (int i = 0; i < NUM_BLOCKS; i++) {
        if (blocks[i]) {
            mini_free(blocks[i]);
        }
    }
    mini_heap_stats();

    return 0;
}