#version 460 core

layout(location = 0) in vec3 position;

uniform mat4 viewProjectionMatrix;
uniform mat4 modelMatrix;

out vec3 pos;


void main() {
    pos = position;

    gl_Position = viewProjectionMatrix * modelMatrix * vec4(position, 1.0);
}
