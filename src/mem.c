#include "mem.h"
#include "int_types.h"

memory mem;

u8 total_memory[MEM_SIZE];

void init_memory() {
    mem.data_stack = total_memory + MEM_SIZE;
}