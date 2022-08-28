#version 420 core

uniform vec2 uvs[6];
uniform ivec4 uv_bounds[6];
uniform mat4 mvp;

layout(location = 0) in uint packed_pos_attr;
layout(location = 1) in uint uv_index_attr;

out vec2 in_uv;
flat out ivec4 in_uv_bounds;

void main() {
    gl_Position = mvp * vec4(
        float(packed_pos_attr & 0x000000ffu),
        float((packed_pos_attr & 0x0000ff00u) >> 0x8u),
        float(packed_pos_attr >> 0x10u),
        1
    );
    in_uv = uvs[uv_index_attr];
    in_uv_bounds = uv_bounds[uv_index_attr];
}