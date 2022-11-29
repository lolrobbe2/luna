#version 460
//#extension GL_ARB_separate_shader_objects : enable
 #extension GL_ARB_shader_draw_parameters : enable
layout(location = 0) in vec3 inPosition;

void main()
{
	//const array of positions for the triangle
	//output the position of each vertex
	//gl_Position = vec4(positions[gl_VertexIndex], 1.0f);
	gl_Position = vec4(inPosition,1.0f);
}