#version 330 core

in vec2 in_vertex_pos;

out vec3 out_color;

void main() {
    out_color = vec3((in_vertex_pos.x + 1) / 2, (in_vertex_pos.y + 1) / 2, (-in_vertex_pos.y + 1) / 2);
}