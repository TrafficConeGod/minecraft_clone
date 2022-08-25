#version 330 core

uniform sampler2D tex_sampler;

in vec2 in_uv;

out vec4 out_color;

void main() {
    out_color = texture(tex_sampler, in_uv).rgba;
}