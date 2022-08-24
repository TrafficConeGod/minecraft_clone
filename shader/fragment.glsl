#version 330 core

in vec2 in_uv;

out vec3 out_color;

void main() {
    out_color = vec3(in_uv.x, in_uv.y, (1 - in_uv.y));
}