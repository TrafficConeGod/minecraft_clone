#include "shader.h"
#include "util.h"
#include <GLFW/glfw3.h>
#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>
#include <cglm/cglm.h>
#include <cglm/struct.h>

static vec3 vertex_positions[] = {
    { -1, -1, 0 },
    { 1, -1, 0 },
    { -1, 1, 0 },
    //
    { 1, 1, 0 },
    { 1, -1, 0 },
    { -1, 1, 0 }
};

static vec2 vertex_uvs[] = {
    { 0, 0 },
    { 1, 0 },
    { 0, 1 },
    //
    { 1, 1 },
    { 1, 0 },
    { 0, 1 }
};

int main() {
    if (!glfwInit()) {
        puts("Failed to initialize GLFW");
        exit(-1);
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
        exit(-1);
    }   
    glfwMakeContextCurrent(win);
    glewExperimental = true;
    if (glewInit() != GLEW_OK) {
        puts("Failed to initialize GLEW");
        exit(-1);
    }

    glfwSetInputMode(win, GLFW_STICKY_KEYS, GL_TRUE);

    glDisable(GL_CULL_FACE);

    GLuint vert_array;
    glGenVertexArrays(1, &vert_array);
    glBindVertexArray(vert_array);

    #define NUM_SHADER_PROGRAMS SIZEOF_ARRAY(shader_path_pairs)

    shader_path_pairs shader_path_pairs[] = {
        { "shader/vertex.glsl", "shader/fragment.glsl" }
    };
    GLuint shader_programs[NUM_SHADER_PROGRAMS];
    error_t code = load_shader_programs(NUM_SHADER_PROGRAMS, shader_path_pairs, shader_programs);
    if (code != 0) {
        printf("Error: %s\n", strerror(code));
        exit(-1);
    }

    #undef NUM_SHADER_PROGRAMS

    glUseProgram(shader_programs[0]);

    mat4s proj = glms_perspective(M_TAU / 4, 1280.0f/720.0f, 0.01f, 300.0f);

    mat4s view = glms_lookat(VEC3(0.0f, 0.0f, -5.0f), VEC3(0.0f, 0.0f, 0.0f), VEC3(0.0f, 1.0f, 0.0f));

    mat4s view_proj = glms_mat4_mul(proj, view);

    mat4s model = glms_mat4_identity();
    model = glms_translate(model, VEC3(0.0f, 0.0f, 0.0f));

    mat4s model_view_proj = glms_mat4_mul(view_proj, model);

    GLuint mvp_uniform = glGetUniformLocation(shader_programs[0], "mvp");
    glUniformMatrix4fv(mvp_uniform, 1, GL_FALSE, (GLfloat*)&model_view_proj);

    GLuint pos_buf;
    glGenBuffers(1, &pos_buf);
    glBindBuffer(GL_ARRAY_BUFFER, pos_buf);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertex_positions), vertex_positions, GL_STATIC_DRAW);

    GLuint uv_buf;
    glGenBuffers(1, &uv_buf);
    glBindBuffer(GL_ARRAY_BUFFER, uv_buf);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertex_uvs), vertex_uvs, GL_STATIC_DRAW);

    for (;;) {
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        glEnableVertexAttribArray(0);
        glBindBuffer(GL_ARRAY_BUFFER, pos_buf);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, NULL);

        glEnableVertexAttribArray(1);
        glBindBuffer(GL_ARRAY_BUFFER, uv_buf);
        glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, NULL);

        glDrawArrays(GL_TRIANGLES, 0, SIZEOF_ARRAY(vertex_positions) * 2);

        glDisableVertexAttribArray(0);
        glDisableVertexAttribArray(1);

        glfwSwapBuffers(win);

        glfwPollEvents();

        if (glfwWindowShouldClose(win)) {
            break;
        }
    }

    return 0;
}