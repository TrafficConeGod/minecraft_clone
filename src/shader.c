#include "shader.h"
#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <errno.h>

error_t load_shader_programs(size_t num_shader_programs, const shader_path_pairs shader_path_pairs[], GLuint shader_programs[]) {
    struct {
        struct stat vertex;
        struct stat fragment;
    } stat_pairs[num_shader_programs];
    for (size_t i = 0; i < num_shader_programs; i++) {
        if (stat(shader_path_pairs[i].vertex, &stat_pairs[i].vertex) != 0) {
            return errno;
        }
        if (stat(shader_path_pairs[i].fragment, &stat_pairs[i].fragment) != 0) {
            return errno;
        }
    }

    struct {
        FILE* vertex;
        FILE* fragment;
    } file_pairs[num_shader_programs];
    for (size_t i = 0; i < num_shader_programs; i++) {
        file_pairs[i].vertex = fopen(shader_path_pairs[i].vertex, "r");
        file_pairs[i].fragment = fopen(shader_path_pairs[i].fragment, "r");
    }

    for (size_t i = 0; i < num_shader_programs; i++) {
        size_t vertex_source_size = stat_pairs[i].vertex.st_size;
        char vertex_source[vertex_source_size + 1];
        vertex_source[vertex_source_size] = '\0';

        fread(vertex_source, vertex_source_size, 1, file_pairs[i].vertex);
        GLuint vertex_shader = glCreateShader(GL_VERTEX_SHADER);
        const char* vertex_source_ptr = vertex_source;
        glShaderSource(vertex_shader, 1, &vertex_source_ptr, NULL);
        glCompileShader(vertex_shader);

        GLint info_log_size;

        glGetShaderiv(vertex_shader, GL_INFO_LOG_LENGTH, &info_log_size);
        if (info_log_size > 0) {
            char info_log[info_log_size + 1];
            info_log[info_log_size] = '\0';
            glGetShaderInfoLog(vertex_shader, info_log_size, NULL, info_log);
            printf("%s\n", info_log);
        }

        size_t fragment_source_size = stat_pairs[i].fragment.st_size;
        char fragment_source[fragment_source_size + 1];
        fragment_source[fragment_source_size] = '\0';

        fread(fragment_source, fragment_source_size, 1, file_pairs[i].fragment);
        GLuint fragment_shader = glCreateShader(GL_FRAGMENT_SHADER);
        const char* fragment_source_ptr = fragment_source;
        glShaderSource(fragment_shader, 1, &fragment_source_ptr, NULL);
        glCompileShader(fragment_shader);

        glGetShaderiv(fragment_shader, GL_INFO_LOG_LENGTH, &info_log_size);
        if (info_log_size > 0) {
            char info_log[info_log_size + 1];
            info_log[info_log_size] = '\0';
            glGetShaderInfoLog(fragment_shader, info_log_size, NULL, info_log);
            printf("%s\n", info_log);
        }

        GLuint shader_program = glCreateProgram();
        glAttachShader(shader_program, vertex_shader);
        glAttachShader(shader_program, fragment_shader);
        glLinkProgram(shader_program);

        glGetProgramiv(shader_program, GL_INFO_LOG_LENGTH, &info_log_size);
        if (info_log_size > 0) {
            char info_log[info_log_size + 1];
            info_log[info_log_size] = '\0';
            glGetProgramInfoLog(shader_program, info_log_size, NULL, info_log);
            printf("%s\n", info_log);
        }

        glDetachShader(shader_program, vertex_shader);
        glDetachShader(shader_program, fragment_shader);

        glDeleteShader(vertex_shader);
        glDeleteShader(fragment_shader);

        shader_programs[i] = shader_program;
    }


    for (size_t i = 0; i < num_shader_programs; i++) {
        fclose(file_pairs[i].vertex);
        fclose(file_pairs[i].fragment);
    }

    return 0;
}