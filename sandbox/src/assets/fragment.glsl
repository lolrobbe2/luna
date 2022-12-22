#version 460
#extension GL_ARB_shader_draw_parameters : enable




layout(location = 0) out vec4 outColor;

layout(location = 0) in vec4 fragColor;


void main() 
{
	outColor = fragColor;


}