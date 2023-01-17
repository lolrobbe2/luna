#version 460
//#extension GL_ARB_separate_shader_objects : enable
 #extension GL_ARB_shader_draw_parameters : enable
layout(location = 0) in vec4 inPosition;
layout(location = 1) in vec4 inColor;
layout(location = 3) in vec3 textureCoord;
layout(location = 0) out vec4 fragColor;
layout(location = 1) out vec3 textCoord;
void main()
{
	fragColor = inColor;
	textCoord = textureCoord;
	gl_Position = inPosition;
}