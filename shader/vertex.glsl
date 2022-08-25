#version 330 core

uniform mat4 mvp;

layout(location = 0) in vec3 attr_pos;
layout(location = 1) in vec2 attr_uv;
layout(location = 2) in vec4 attr_uv_bounds;

out vec2 in_uv;
flat out vec4 in_uv_bounds;

void main() {
    gl_Position = mvp * vec4(attr_pos, 1);
    in_uv = attr_uv;
    in_uv_bounds = attr_uv_bounds;
}