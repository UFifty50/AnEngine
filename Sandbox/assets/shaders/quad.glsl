#type vertex
#version 460 core

layout(location = 0) in vec3 position;
layout(location = 1) in vec2 TexCoordIn;

out vec2 TexCoord;

uniform mat4 viewProjectionMatrix;
uniform mat4 modelMatrix;
uniform float tilingFactor;


void main() {
    TexCoord = TexCoordIn * tilingFactor;
    gl_Position = viewProjectionMatrix * modelMatrix * vec4(position, 1.0);
}


#type pixel
#version 460 core

layout(location = 0) out vec4 colour;

in vec2 TexCoord;

uniform vec4 Ucolour;
uniform vec4 tint;
uniform sampler2D textureSampler;


void main() {
    colour = texture(textureSampler, TexCoord) * Ucolour * tint;
}
