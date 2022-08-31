#include "blocks.h"
#include <stdbool.h>
#include <cglm/struct.h>

static bool is_face_visible(size_t index, size_t nb_index, const block_type types[]) {
    switch (types[index]) {
        default: return false;
        case grass_block: return types[nb_index] == air_block;
    }
}

static bool front_visibilities[NUM_BLOCKS];
static bool back_visibilities[NUM_BLOCKS];
static bool top_visibilities[NUM_BLOCKS];
static bool bottom_visibilities[NUM_BLOCKS];
static bool right_visibilities[NUM_BLOCKS];
static bool left_visibilities[NUM_BLOCKS];

void create_block_meshes_from_block_types(const block_type types[]) {
    // Using a macro here since it saves a JMP instruction in the is_face_visible function, while not polluting the i-cache inside of the block loop. However, it should be noted that this does take up extra i-cache space outside of this loop and therefore may affect performance initially entering into a block loop.
    #define FACE_CASE(face_visibilities, nb_begin_index, is_face_visible) { \
        size_t index = 0; \
        size_t nb_index = nb_begin_index; \
        /* Initialize nb_index such: 0 <= nb_index < NUM_BLOCKS */ \
        if (nb_index >= NUM_BLOCKS) { \
            index -= nb_index; \
            nb_index = 0; \
        } \
        \
        size_t end_size = NUM_BLOCKS - index; \
        while (nb_index < end_size) { \
            face_visibilities[index] = is_face_visible(index, nb_index, types); \
            index++; \
            nb_index++; \
        } \
    }

    // Checking faces that have a negative nb_begin_index first so that it is slightly more likely that the positive nb_begin_index face will have a cache hit
    FACE_CASE(back_visibilities, -CHUNK_X_OFFSET, is_face_visible)
    FACE_CASE(front_visibilities, CHUNK_X_OFFSET, is_face_visible)
    FACE_CASE(bottom_visibilities, -CHUNK_Y_OFFSET, is_face_visible)
    FACE_CASE(top_visibilities, CHUNK_Y_OFFSET, is_face_visible)
    FACE_CASE(left_visibilities, -CHUNK_Z_OFFSET, is_face_visible)
    FACE_CASE(right_visibilities, CHUNK_Z_OFFSET, is_face_visible)
}

void populate_block_arrays(size_t num_arrays, const vec3 chunk_positions[], block_type* const type_arrays[]) {
    for (size_t i = 0; i < num_arrays; i++) {
        vec3s pos = { .x = chunk_positions[i][0], .y = chunk_positions[i][1], .z = chunk_positions[i][2] };
        block_type* types = type_arrays[i];

        for (size_t j = 0; j < NUM_BLOCKS; j++) {
            types[j] = grass_block;
        }
    }
}