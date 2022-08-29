#pragma once
#include <stddef.h>

#define MEM_SIZE 33554432 // 32 MiB

typedef struct {
    void* data_stack;
} memory;

extern memory mem;

void init_memory();
void term_memory();

void push_dynamic_values_onto_data_stack(size_t num_values, const size_t value_byte_counts[], void* begin_ptrs[]);
void pop_dynamic_values_from_data_stack(size_t num_values);