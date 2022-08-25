#pragma once
#include "fs.h"
#include <GL/glew.h>

error_t load_png_textures(size_t num_textures, FILE* const files[], GLuint textures[]);