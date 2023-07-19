#type vertex
#version 460 core

layout(location = 0) in vec3 position;

uniform mat4 viewProjectionMatrix;
uniform mat4 modelMatrix;

out vec3 pos;


void main() {
    pos = position;

    gl_Position = viewProjectionMatrix * modelMatrix * vec4(position, 1.0);
}

#type fragment
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