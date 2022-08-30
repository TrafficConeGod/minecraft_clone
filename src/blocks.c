#include "blocks.h"
#include "mem.h"
#include <stdbool.h>
#include <cglm/struct.h>

static size_t get_face_visibility_offset(size_t face) {
    return sizeof(bool) * NUM_BLOCKS * face;
}

static size_t get_neighbor_begin_index(size_t face) {
    switch (face) {
        case FRONT_FACE: return CHUNK_X_OFFSET;
        case BACK_FACE: return -CHUNK_X_OFFSET;
        case TOP_FACE: return CHUNK_Y_OFFSET;
        case BOTTOM_FACE: return -CHUNK_Y_OFFSET;
        case RIGHT_FACE: return CHUNK_Z_OFFSET;
        case LEFT_FACE: return -CHUNK_Z_OFFSET;
    }
    return 0;
}

// TODO: Implement functions for seperate faces
static bool is_face_visible(size_t index, size_t nb_index, const block_type types[]) {
    switch (types[index]) {
        default: return false;
        case grass_block: return types[nb_index] == air_block;
    }
}

void create_block_meshes_from_block_types(const block_type types[]) {
    mem.data_stack -= get_face_visibility_offset(NUM_FACES);

    bool* face_visibility_arrays[NUM_FACES];
    for (size_t face = 0; face < NUM_FACES; face++) {
        face_visibility_arrays[face] = mem.data_stack + get_face_visibility_offset(face);
    }

    for (size_t face = 0; face < NUM_FACES; face++) {
        bool* face_visibilities = face_visibility_arrays[face];
        size_t index = 0;
        size_t nb_index = get_neighbor_begin_index(face);
        // Initialize nb_index such: 0 <= nb_index < NUM_BLOCKS
        if (nb_index >= NUM_BLOCKS) {
            index -= nb_index;
            nb_index = 0;
        }

        size_t end_index = NUM_BLOCKS - index;
        while (nb_index < end_index) {
            face_visibilities[index] = is_face_visible(index, nb_index, types);
            index++;
            nb_index++;
        }
    }

    mem.data_stack += get_face_visibility_offset(NUM_FACES);
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