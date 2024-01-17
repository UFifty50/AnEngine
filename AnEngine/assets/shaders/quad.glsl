#type vertex
#version 460 core

layout(location = 0) in vec3 Position;
layout(location = 1) in vec4 ColourIn;
layout(location = 2) in vec2 TexCoordIn;
layout(location = 3) in float TexIndexIn;
layout(location = 4) in float TilingFactorIn;
layout(location = 5) in vec4 TintIn;
//layout(location = 6) in int EntityIDin;


out vec4 Colour;
out vec2 TexCoord;
out flat float TexIndex;
out vec4 Tint;
//out flat int EntityID;

uniform mat4 viewProjectionMatrix;


void main() {
    Colour = ColourIn;
    TexCoord = TexCoordIn * TilingFactorIn;
    TexIndex = TexIndexIn;
    Tint = TintIn;
  //  EntityID = EntityIDin;
    gl_Position = viewProjectionMatrix * vec4(Position, 1.0);
}


#type pixel
#version 460 core

layout(location = 0) out vec4 colour;
layout(location = 1) out int colour2;

in vec4 Colour;
in vec2 TexCoord;
in flat float TexIndex;
in vec4 Tint;
//in flat int EntityID;

uniform sampler2D textureSamplers[32];
	

void main() {
    // maybe int switch() or if() for texture index
    vec4 texColour = texture(textureSamplers[int(TexIndex)], TexCoord) * Colour * Tint;
    if(texColour.a < 0.05)
        discard;

    colour = texColour;
    colour2 = 50;
}
