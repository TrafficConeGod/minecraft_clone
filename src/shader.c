#include "shader.h"

error_t load_shader_programs(size_t num_shader_programs, const shader_stat_pair stats[], const shader_file_pair files[], GLuint shader_programs[]) {
    for (size_t i = 0; i < num_shader_programs; i++) {
        size_t vertex_source_size = stats[i][0].st_size;
        char vertex_source[vertex_source_size + 1];
        vertex_source[vertex_source_size] = '\0';

        if (fread(vertex_source, vertex_source_size, 1, files[i][0]) != 1) {
            return -1;
        }
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

        size_t fragment_source_size = stats[i][1].st_size;
        char fragment_source[fragment_source_size + 1];
        fragment_source[fragment_source_size] = '\0';

        if (fread(fragment_source, fragment_source_size, 1, files[i][1]) != 1) {
            return -1;
        }
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
    return 0;
}

void unload_shader_programs(size_t num_shader_programs, GLuint shader_programs[]) {
    for (size_t i = 0; i < num_shader_programs; i++) {
        glDeleteShader(shader_programs[i]);
    }
}