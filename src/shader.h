#pragma once
#include "fs.h"
#include <GL/glew.h>
#include <stdint.h>

typedef struct stat shader_stat_pair[2];
typedef FILE* shader_file_pair[2];

error_t load_shader_programs(size_t num_shader_programs, const shader_stat_pair stats[], const shader_file_pair files[], GLuint shader_programs[]);
void unload_shader_programs(size_t num_shader_programs, GLuint shader_programs[]);

typedef const char* shader_path_pair[2];