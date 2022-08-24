#version 330 core

layout(location = 0) in vec2 attr_vertex_pos;

out vec2 in_vertex_pos;

void main() {
    gl_Position = vec4(attr_vertex_pos, 0.0, 1.0);
    in_vertex_pos = attr_vertex_pos;
}