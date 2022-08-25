#version 330 core

uniform sampler2D tex_sampler;

in vec2 in_uv;

out vec4 out_color;

void main() {
    ivec2 rounded_uv = ivec2(in_uv.x, in_uv.y);
    out_color = texelFetch(tex_sampler, rounded_uv, 0).rgba;
}