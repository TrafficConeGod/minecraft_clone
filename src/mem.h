#pragma once

#define MEM_SIZE 33554432 // 32 MiB

typedef struct {
    void* data_stack;
} memory;

extern memory mem;

void init_memory();