#version 460 core

layout(location = 0) out vec4 colour;

in vec2 TexCoord0;

uniform vec3 Ucolour;

void main() {
    colour = vec4(TexCoord0, 0.0, 1.0f);
}
