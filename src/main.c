#include "shader.h"
#include "mem.h"
#include "texture.h"
#include "fs.h"
#include "gfx.h"
#include "util.h"
#include "blocks.h"
#include <GLFW/glfw3.h>
#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>
#include <cglm/cglm.h>
#include <cglm/struct.h>

static const vec2 uvs[] = {
    { 0, 0 },
    { 16, 0 },
    { 0, 16 },
    { 16, 16 },
    { 16, 0 },
    { 0, 16 }
};

static const ivec4 uv_bounds[] = {
    { 0, 15, 0, 15 },
    { 0, 15, 0, 15 },
    { 0, 15, 0, 15 },
    { 0, 15, 0, 15 },
    { 0, 15, 0, 15 },
    { 0, 15, 0, 15 }
};

static const pos_attr vertex_positions[] = {
    { 0, 0, 0 },
    { 16, 0, 0 },
    { 0, 16, 0 },
    //
    { 16, 16, 0 },
    { 16, 0, 0 },
    { 0, 16, 0 },
    ///
    { 16, 0, 0 },
    { 32, 0, 0 },
    { 16, 16, 0 },
    //
    { 32, 16, 0 },
    { 32, 0, 0 },
    { 16, 16, 0 },
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

    glUniform2fv(glGetUniformLocation(shader_programs[0], "uvs"), SIZEOF_ARRAY(uvs), (const GLfloat*)uvs);
    glUniform4iv(glGetUniformLocation(shader_programs[0], "uv_bounds"), SIZEOF_ARRAY(uv_bounds), (const GLint*)uv_bounds);
    glUniformMatrix4fv(glGetUniformLocation(shader_programs[0], "mvp"), 1, GL_FALSE, (const GLfloat*)&model_view_proj);

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, textures[0]);
    glUniform1i(glGetUniformLocation(shader_programs[0], "tex_sampler"), 0);

    mem.data_stack -= sizeof(block_type) * NUM_BLOCKS;

    block_type* block_types = mem.data_stack;
    vec3 chunk_pos = { 0, 0, 0 };
    populate_block_arrays(1, &chunk_pos, &block_types);
    
    create_block_meshes_from_block_types(block_types);

    mem.data_stack += sizeof(block_type) * NUM_BLOCKS;
    
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
        return 0;
}