#version 420 core

uniform mat4 mvp;

struct uv_elem {
    vec2 uv;
    ivec4 uv_bounds;
} uv_elems[6] = {
    { { 48, 0 }, { 48, 63, 0, 15 } },
    { { 64, 0 }, { 48, 63, 0, 15 } },
    { { 48, 16 }, { 48, 63, 0, 15 } },
    { { 64, 16 }, { 48, 63, 0, 15 } },
    { { 64, 0 }, { 48, 63, 0, 15 } },
    { { 48, 16 }, { 48, 63, 0, 15 } }
};

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
    uv_elem elem = uv_elems[uv_index_attr];
    in_uv = elem.uv;
    in_uv_bounds = elem.uv_bounds;
}