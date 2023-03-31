#version 330 core

layout(location = 0) out vec4 colour;

in vec3 pos;


void main() {
    colour = vec4(pos*0.5+0.5, 1.0);
}
