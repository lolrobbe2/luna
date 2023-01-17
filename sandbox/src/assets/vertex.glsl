#version 460
//#extension GL_ARB_separate_shader_objects : enable
 #extension GL_ARB_shader_draw_parameters : enable
layout(location = 0) in vec4 inPosition;
layout(location = 1) in vec4 inColor;
layout(location = 3) in vec2 textureCoord;
layout(location = 0) out vec4 fragColor;
layout(location = 1) out vec2 textCoord;

void main()
{
	fragColor = inColor;
	gl_Position = inPosition;
}