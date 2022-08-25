#version 330 core

uniform sampler2D tex_sampler;

in vec2 in_uv;

out vec4 out_color;

void main() {
    ivec2 rounded_uv = ivec2(clamp(in_uv.x, 48, 63), clamp(in_uv.y, 0, 15));
    out_color = texelFetch(tex_sampler, rounded_uv, 0).rgba;
}