#include "mem.h"
#include <stdlib.h>
#include <math.h>

#define MEM_SIZE 33554432 // 32 MiB

memory mem;

void* total_memory;

void init_memory() {
    total_memory = malloc(MEM_SIZE);
    mem.data_stack = total_memory + MEM_SIZE;
}

void term_memory() {
    free(total_memory);
}