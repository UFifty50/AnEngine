#version 460 core

layout(location = 0) out vec4 colour;

in vec2 TexCoord0;

uniform sampler2D Utexture;

void main() {
    colour = texture(Utexture, TexCoord0);
}
