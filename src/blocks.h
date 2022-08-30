#pragma once
#include "gfx.h"
#include "util.h"
#include <cglm/cglm.h>

#define CHUNK_SIZE 16
#define CHUNK_X_OFFSET 1
#define CHUNK_Y_OFFSET CHUNK_SIZE
#define CHUNK_Z_OFFSET (CHUNK_SIZE * CHUNK_SIZE)
#define NUM_BLOCKS (CHUNK_SIZE * CHUNK_SIZE * CHUNK_SIZE)

typedef enum PACKED {
    air_block,
    grass_block
} block_type;

#define NUM_FACES 6

#define FRONT_FACE 0
#define BACK_FACE 1
#define TOP_FACE 2
#define BOTTOM_FACE 3
#define RIGHT_FACE 4
#define LEFT_FACE 5

void populate_block_arrays(size_t num_arrays, const vec3 chunk_positions[], block_type* const type_arrays[]);
void create_block_meshes_from_block_types(const block_type types[]);