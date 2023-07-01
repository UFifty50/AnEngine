#version 460 core

layout(location = 0) in vec3 position;
//layout(location = 1) in vec4 colour;

out vec3 pos;
//out vec4 col;


void main() {
    pos = position;
  //  col = colour;

    gl_Position = vec4(position, 1.0);
}
