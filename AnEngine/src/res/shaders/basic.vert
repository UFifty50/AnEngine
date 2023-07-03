#version 460 core

layout(location = 0) in vec3 position;

uniform mat4 viewProjection;

out vec3 pos;


void main() {
    pos = position;

    gl_Position = viewProjection * vec4(position, 1.0);
}
