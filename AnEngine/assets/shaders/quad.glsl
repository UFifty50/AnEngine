#type vertex
#version 460 core

layout(location = 0) in vec3 Position;
layout(location = 1) in vec4 ColourIn;
layout(location = 2) in vec2 TexCoordIn;
layout(location = 3) in float TexIndexIn;
layout(location = 4) in float TilingFactorIn;
layout(location = 5) in vec4 TintIn;
layout(location = 6) in int EntityIDin;

layout(std140, binding = 0) uniform Camera {
    mat4 viewProjectionMatrix;
};


// All scalar and non-double vector types take up 1 slot
struct VertexOutput {
    vec4 Colour;
    vec2 TexCoord;
    float TexIndex;
    float TilingFactor;
    vec4 Tint;
};

layout(location = 0) out VertexOutput Output;
layout(location = 5) out flat int EntityID;


void main() {
    Output.Colour = ColourIn;
    Output.TexCoord = TexCoordIn * TilingFactorIn;
    Output.TexIndex = TexIndexIn;
    Output.TilingFactor = TilingFactorIn;
    Output.Tint = TintIn;
    EntityID = EntityIDin;

    gl_Position = viewProjectionMatrix * vec4(Position, 1.0);
}


#type pixel
#version 460 core

layout(location = 0) out vec4 colour;
layout(location = 1) out int colour2;

struct VertexOutput {
    vec4 Colour;
    vec2 TexCoord;
    float TexIndex;
    float TilingFactor;
    vec4 Tint;
};

layout(location = 0) in VertexOutput Input;
layout(location = 5) in flat int EntityID;

layout(binding = 0) uniform sampler2D textureSamplers[32];
	

void main() {
    // maybe int switch() or if() for texture index
    vec4 texColour = Input.Colour * Input.Tint * texture(textureSamplers[int(Input.TexIndex)], Input.TexCoord);
    if(texColour.a < 0.05)
        discard;

    colour = texColour;
    colour2 = EntityID;
}
