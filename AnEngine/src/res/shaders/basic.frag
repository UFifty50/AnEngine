#version 460 core

layout(location = 0) out vec4 colour;

uniform vec4 Ucolour;

in vec3 pos;
in vec4 col;


void main() {
    //colour = vec4(pos*0.5+0.5, 1.0);
    colour = Ucolour;
    //colour = col;
}