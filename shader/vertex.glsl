#version 330 core

layout(location = 0) in vec2 attr_pos;
layout(location = 1) in vec2 attr_uv;

out vec2 in_uv;

void main() {
    gl_Position = vec4(attr_pos, 0.0, 1.0);
    in_uv = attr_uv;
}