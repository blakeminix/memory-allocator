#include "mini_malloc.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>

#define ALIGN(x) (((((x) - 1) >> 2) << 2) + 4)

typedef struct block {
    size_t size;
    int free;
    struct block* next;
} block_t;

static unsigned char* heap = NULL;
static block_t* free_list = NULL;
static size_t heap_size = 0;

void mini_heap_init(size_t size) {
    heap = (unsigned char*) malloc(size);
    if (!heap) {
        fprintf(stderr, "Heap init failed\n");
        exit(1);
    }
    heap_size = size;

    free_list = (block_t*) heap;
    free_list->size = size - sizeof(block_t);
    free_list->free = 1;
    free_list->next = NULL;
}

void* mini_malloc(size_t size) {
    size = ALIGN(size);
    block_t* curr = free_list;

    while (curr) {
        if (curr->free && curr->size >= size) {
            if (curr->size > size + sizeof(block_t)) {
                block_t* new_block = (block_t*) ((unsigned char*) curr + sizeof(block_t) + size);
                new_block->size = curr->size - size - sizeof(block_t);
                new_block->free = 1;
                new_block->next = curr->next;

                curr->size = size;
                curr->next = new_block;
            }
            curr->free = 0;
            return (unsigned char*) curr + sizeof(block_t);
        }
        curr = curr->next;
    }

    return NULL;
}

void mini_free(void* ptr) {
    if (!ptr) return;

    block_t* block = (block_t*) ((unsigned char*) ptr - sizeof(block_t));
    if (block->free) {
        return;
    }
    block->free = 1;

    block_t* curr = free_list;
    while (curr && curr->next) {
        if (curr->free && curr->next->free) {
            curr->size += sizeof(block_t) + curr->next->size;
            curr->next = curr->next->next;
        } else {
            curr = curr->next;
        }
    }
}

void mini_heap_stats() {
    block_t* curr = free_list;
    size_t used = 0;
    size_t free = 0;
    size_t largest_free = 0;
    int allocs = 0;
    int frees = 0;

    while (curr) {
        if (curr->free) {
            free += curr->size;
            frees++;
            if (curr->size > largest_free) {
                largest_free = curr->size;
            }
        } else {
            used += curr->size;
            allocs++;
        }
        curr = curr->next;
    }

    float fragmentation = 0;
    if (free > 0) {
        fragmentation = 100.0f * (1.0f - ((float) largest_free / (float) free));
    }

    printf("Mini Heap Stats:\n");
    printf("Heap Size: %zu bytes\n", heap_size);
    printf("Used Memory: %zu bytes\n", used);
    printf("Free Memory: %zu bytes\n", free);
    printf("Allocations: %d\n", allocs);
    printf("Free Blocks: %d\n", frees);
    printf("Largest Free Block: %zu bytes\n", largest_free);
    printf("Fragmentation: %.2f%%\n", fragmentation);
}