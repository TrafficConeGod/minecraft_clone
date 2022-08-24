#version 330 core

in vec2 in_color;

out vec3 out_color;

void main() {
    out_color = vec3(in_color.x, in_color.y, 0);
}