#pragma once
#include <GL/glew.h>
#include <stdint.h>
#include <bits/types/error_t.h>

typedef struct {
    const char* vertex;
    const char* fragment;
} shader_path_pairs;

error_t load_shader_programs(size_t num_shader_programs, const shader_path_pairs shader_path_pairs[], GLuint shader_programs[]);