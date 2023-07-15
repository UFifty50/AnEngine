#shader vertex
#version 460 core

layout(location = 0) in vec3 Position;
layout(location = 1) in vec2 TexCoord;

uniform mat4 viewProjectionMatrix;
uniform mat4 modelMatrix;

out vec2 TexCoord0;


void main() {
    TexCoord0 = TexCoord;
    gl_Position = viewProjectionMatrix * modelMatrix * vec4(Position, 1.0);
}


#shader fragment
#version 460 core

layout(location = 0) out vec4 colour;

in vec2 TexCoord0;

uniform sampler2D Utexture;

void main() {
    colour = texture(Utexture, TexCoord0);
}
