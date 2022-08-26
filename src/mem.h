#pragma once

typedef struct {
    void* data_stack;
} memory;

extern memory mem;

void init_memory();
void term_memory();