#pragma once
#include "fs.h"
#include <stdint.h>
#include <GL/glew.h>

typedef struct {
    size_t width;
    size_t height;
    GLubyte* data;
} image;

error_t load_png_files(size_t num_textures, FILE* const files[], image images[]);
void load_textures(size_t num_textures, const image images[], GLuint textures[]);