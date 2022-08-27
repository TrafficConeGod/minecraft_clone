#version 330 core

uniform mat4 mvp;

layout(location = 0) in uint packed_pos_attr;
layout(location = 1) in uint packed_uv_attr;
layout(location = 2) in uint packed_uv_bounds_attr;

out vec2 in_uv;
flat out ivec4 in_uv_bounds;

void main() {
    gl_Position = mvp * vec4(
        float(packed_pos_attr & 0x000000ffu),
        float((packed_pos_attr & 0x0000ff00u) >> 0x8u),
        float(packed_pos_attr >> 0x10u),
        1
    );
    in_uv = vec2(
        float(packed_uv_attr & 0x000000ffu),
        float((packed_uv_attr & 0x0000ff00u) >> 0x8u)
    );
    in_uv_bounds = ivec4(
        packed_uv_bounds_attr & 0x000000ffu,
        (packed_uv_bounds_attr & 0x0000ff00u) >> 0x8u,
        (packed_uv_bounds_attr & 0x00ff0000u) >> 0x10u,
        packed_uv_bounds_attr >> 0x18u
    );
}