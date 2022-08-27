#version 330 core

uniform sampler2D tex_sampler;

in vec2 in_uv;
flat in ivec4 in_uv_bounds;

out vec4 out_color;

void main() {
    ivec2 uv = ivec2(in_uv.x, in_uv.y);
    uv = ivec2(clamp(uv.x, in_uv_bounds.x, in_uv_bounds.y), clamp(uv.y, in_uv_bounds.z, in_uv_bounds.w));
    out_color = texelFetch(tex_sampler, uv, 0).rgba;
}