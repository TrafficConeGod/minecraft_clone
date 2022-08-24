#version 330 core

uniform mat4 mvp;

layout(location = 0) in vec3 attr_pos;
layout(location = 1) in vec2 attr_uv;

out vec2 in_uv;

void main() {
    gl_Position = mvp * vec4(attr_pos, 1);
    in_uv = attr_uv;
}