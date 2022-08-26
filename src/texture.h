#pragma once
#include "fs.h"
#include <stdint.h>
#include <GL/glew.h>

typedef struct {
    size_t width;
    size_t height;
    GLubyte* data;
} image;

error_t load_png_images_onto_data_stack(size_t num_images, FILE* const files[], image images[]);
void load_textures(size_t num_textures, const image images[], GLuint textures[]);
void free_images_from_data_stack(size_t num_images);