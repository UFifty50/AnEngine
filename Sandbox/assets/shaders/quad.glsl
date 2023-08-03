#type vertex
#version 460 core

layout(location = 0) in vec3 Position;
layout(location = 1) in vec4 ColourIn;
layout(location = 2) in vec2 TexCoordIn;
layout(location = 3) in float TexIndexIn;
layout(location = 4) in float TilingFactorIn;
layout(location = 5) in vec4 TintIn;


out vec4 Colour;
out vec2 TexCoord;
out float TexIndex;
out float TilingFactor;
out vec4 Tint;

uniform mat4 viewProjectionMatrix;
uniform mat4 modelMatrix;


void main() {
    Colour = ColourIn;
    TexCoord = TexCoordIn;// * tilingFactor;
    TexIndex = TexIndexIn;
    TilingFactor = TilingFactorIn;
    Tint = TintIn;
    gl_Position = viewProjectionMatrix * vec4(Position, 1.0); // * modelMatrix;
}


#type pixel
#version 460 core

layout(location = 0) out vec4 colourOut;

in vec4 Colour;
in vec2 TexCoord;
in float TexIndex;
in float TilingFactor;
in vec4 Tint;

uniform sampler2D textureSamplers[32];


void main() {
    // maybe int switch() or if() for texture index
    colourOut = texture(textureSamplers[int(TexIndex)], TexCoord * TilingFactor) * Colour * Tint;
}
