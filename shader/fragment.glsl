#version 330 core

uniform sampler2D tex_sampler;

in vec2 in_uv;
flat in vec4 in_uv_bounds;

out vec4 out_color;

void main() {
    ivec2 rounded_uv = ivec2(clamp(in_uv.x, in_uv_bounds.x, in_uv_bounds.y), clamp(in_uv.y, in_uv_bounds.z, in_uv_bounds.w));
    out_color = texelFetch(tex_sampler, rounded_uv, 0).rgba;
}