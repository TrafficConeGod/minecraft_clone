#include "shader.h"
#include <GLFW/glfw3.h>
#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <stdlib.h>

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

    GLuint vert_array;
    glGenVertexArrays(1, &vert_array);
    glBindVertexArray(vert_array);

    #define NUM_SHADER_PROGRAMS sizeof(shader_path_pairs)/sizeof(shader_path_pairs[0])

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

    return 0;
}