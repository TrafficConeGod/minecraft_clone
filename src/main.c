#include "shader.h"
#include "mem.h"
#include "texture.h"
#include "fs.h"
#include "gfx.h"
#include "util.h"
#include <GLFW/glfw3.h>
#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>
#include <cglm/cglm.h>
#include <cglm/struct.h>

static const pos_attr vertex_positions[] = {
    { 0, 0, 0 },
    { 1, 0, 0 },
    { 0, 1, 0 },
    //
    { 1, 1, 0 },
    { 1, 0, 0 },
    { 0, 1, 0 },
    ///
    { 1, 0, 0 },
    { 2, 0, 0 },
    { 1, 1, 0 },
    //
    { 2, 1, 0 },
    { 2, 0, 0 },
    { 1, 1, 0 },
};

static const u32 vertex_uv_indices[] = {
    0, 1, 2,
    3, 4, 5,
    //
    0, 1, 2,
    3, 4, 5
};

static const char* file_paths[] = {
    "shader/vertex.glsl", "shader/fragment.glsl",
    "textures/chunk.png"
};

#define SHADER_PROGRAMS_BEGIN 0
#define NUM_SHADER_PROGRAMS 1

#define TEXTURES_BEGIN (NUM_SHADER_PROGRAMS * 2)
#define NUM_TEXTURES 1

static const char* file_modes[] = {
    "r", "r",
    "rb"
};

int main() {
    init_memory();

    // Init GLFW
    if (!glfwInit()) {
        puts("Failed to initialize GLFW");
        goto error;
    }
    glfwWindowHint(GLFW_SAMPLES, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    GLFWwindow* win = glfwCreateWindow(1280, 720, "Test", NULL, NULL);
    if (win == NULL) {
        glfwTerminate();
        puts("Failed to open GLFW window");
        goto error;
    }   
    glfwMakeContextCurrent(win);
    glewExperimental = true;
    if (glewInit() != GLEW_OK) {
        puts("Failed to initialize GLEW");
        goto error;
    }

    glfwSetInputMode(win, GLFW_STICKY_KEYS, GL_TRUE);

    GLuint shader_programs[NUM_SHADER_PROGRAMS];
    GLuint textures[NUM_TEXTURES];
    {
        #define NUM_FILES SIZEOF_ARRAY(file_paths)
        struct stat stats[NUM_FILES];
        FILE* files[NUM_FILES];

        // Open all files
        error_t code = open_files(NUM_FILES, file_paths, file_modes, stats, files);
        if (code != 0) {
            printf("Error: %s\n", strerror(code));
            goto error;
        }

        GLuint vert_array;
        glGenVertexArrays(1, &vert_array);
        glBindVertexArray(vert_array);

        // Load shaders
        code = load_shader_programs(NUM_SHADER_PROGRAMS, (const shader_stat_pair*)&stats[0], (const shader_file_pair*)&files[0], shader_programs);
        if (code != 0) {
            printf("Error loading shader program\n");
            goto error;
        }

        image images[NUM_TEXTURES];
        code = load_png_images_onto_data_stack(NUM_TEXTURES, &files[TEXTURES_BEGIN], images);
        if (code != 0) {
            printf("Error loading png file\n");
            goto error;
        }

        load_textures(NUM_TEXTURES, images, textures);

        free_images_from_data_stack(NUM_TEXTURES);

        // Close all files
        close_files(NUM_FILES, files);
        #undef NUM_FILES
    }

    glUseProgram(shader_programs[0]);

    mat4s proj = glms_perspective(M_TAU / 4, 1280.0f/720.0f, 0.01f, 300.0f);

    mat4s view = glms_lookat(VEC3(2.0f, 5.0f, -5.0f), VEC3(0.0f, 0.0f, 0.0f), VEC3(0.0f, 1.0f, 0.0f));

    mat4s view_proj = glms_mat4_mul(proj, view);

    mat4s model = glms_mat4_identity();
    model = glms_translate(model, VEC3(0.0f, 0.0f, 0.0f));

    mat4s model_view_proj = glms_mat4_mul(view_proj, model);

    GLuint mvp_uniform = glGetUniformLocation(shader_programs[0], "mvp");
    glUniformMatrix4fv(mvp_uniform, 1, GL_FALSE, (GLfloat*)&model_view_proj);

    GLuint tex_sampler_uniform = glGetUniformLocation(shader_programs[0], "tex_sampler");
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, textures[0]);
    glUniform1i(tex_sampler_uniform, 0);
    
    union {
        GLuint data[2];
        struct PACKED {
            GLuint pos;
            GLuint uv_indices;
        };
    } buffers;
    
    glGenBuffers(SIZEOF_ARRAY(buffers.data), buffers.data);

    glBindBuffer(GL_ARRAY_BUFFER, buffers.pos);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertex_positions), vertex_positions, GL_STATIC_DRAW);

    glBindBuffer(GL_UNIFORM_BUFFER, buffers.uv_indices);
    glBufferData(GL_UNIFORM_BUFFER, sizeof(vertex_uv_indices), vertex_uv_indices, GL_STATIC_DRAW);

    for (;;) {
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        glEnableVertexAttribArray(0);
        glBindBuffer(GL_ARRAY_BUFFER, buffers.pos);
        glVertexAttribIPointer(0, 1, GL_UNSIGNED_INT, 0, NULL);

        glEnableVertexAttribArray(1);
        glBindBuffer(GL_ARRAY_BUFFER, buffers.uv_indices);
        glVertexAttribIPointer(1, 1, GL_UNSIGNED_INT, 0, NULL);

        glDrawArrays(GL_TRIANGLES, 0, SIZEOF_ARRAY(vertex_positions));

        glDisableVertexAttribArray(0);
        glDisableVertexAttribArray(1);

        glfwSwapBuffers(win);

        glfwPollEvents();

        if (glfwWindowShouldClose(win)) {
            goto end;
        }
    }

    error:
    end:
        term_memory();

    return 0;
}