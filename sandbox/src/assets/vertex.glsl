#version 460
#extension GL_ARB_shader_draw_parameters : enable

layout(set = 0,binding = 2) uniform UniformBufferObject 
{
    mat4 model[32];
    mat4 view[32];
    mat4 proj[32];
    bool text[32];
} ubo;

layout(std140,push_constant) uniform PER_OBJECT
{
	int index;
	int text;
}push;

struct ObjectData{
	mat4 o_model;
    mat4 o_view;
    mat4 o_proj;
    bool o_text;
};

layout(location = 1) out vec2 fragTexCoord;
layout(location = 0) out vec3 fragColor;

void main()
{
    
}