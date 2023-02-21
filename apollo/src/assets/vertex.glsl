#version 460
#extension GL_EXT_nonuniform_qualifier : enable
layout(location = 0) in vec4 inPosition;
layout(location = 1) in vec4 inColor;
layout(location = 2) in vec2 textureCoord;
layout(location = 3) in float textureIndex;

layout(location = 0) out vec4 fragColor;
layout(location = 1) out vec2 textCoord;
layout(location = 2) out float textIndex;
void main()
{
	fragColor = inColor;
	textCoord = textureCoord;
	textIndex = textureIndex;
	gl_Position = inPosition;
}