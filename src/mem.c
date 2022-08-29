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

void push_dynamic_values_onto_data_stack(size_t num_values, const size_t value_byte_counts[], void* begin_ptrs[]) {
    for (size_t i = 0; i < num_values; i++) {
        size_t size = value_byte_counts[i];
        mem.data_stack -= sizeof(size_t) + size;
        begin_ptrs[i] = mem.data_stack + sizeof(size_t);
    }
}

void pop_dynamic_values_from_data_stack(size_t num_values) {
    for (size_t i = 0; i < num_values; i++) {
        size_t size = *(size_t*)mem.data_stack;
        mem.data_stack += sizeof(size_t) + size;
    }
}