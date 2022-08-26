#include "mem.h"
#include <stdlib.h>
#include <math.h>

memory mem;

void* total_memory;

void init_memory() {
    total_memory = malloc(MEM_SIZE);
    mem.data_stack = total_memory + MEM_SIZE;
}

void term_memory() {
    free(total_memory);
}