#type vertex
#version 460 core

layout(location = 0) in vec3 position;

uniform mat4 viewProjectionMatrix;
uniform mat4 modelMatrix;


void main() {
    gl_Position = viewProjectionMatrix * modelMatrix * vec4(position, 1.0);
}


#type pixel
#version 460 core

layout(location = 0) out vec4 colour;

uniform vec4 Ucolour;


void main() {
    colour = Ucolour;
}
