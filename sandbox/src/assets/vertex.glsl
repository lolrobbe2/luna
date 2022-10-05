#version 460
//#extension GL_ARB_separate_shader_objects : enable
 #extension GL_ARB_shader_draw_parameters : enable

layout(binding = 0) uniform UniformBufferObject 
{
    mat4 model[32];
    mat4 view[32];
    mat4 proj[32];
    bool text[32];
} ubo;

struct ObjectData{
	mat4 model;
    mat4 view;
    mat4 proj;
    bool text;
};





layout(location = 0) in vec2 inPosition;
layout(location = 1) in vec3 inColor;
layout(location = 2) in vec2 inTexCoord;



layout(location = 0) out vec3 fragColor;
layout(location = 1) out vec2 fragTexCoord;

layout(std140,set = 0, binding = 3) buffer ObjectBuffer{

	ObjectData objects[];
} objectBuffer;
void main() 
{
 gl_Position = ubo.proj[0] * objectBuffer.objects[gl_InstanceIndex].proj * objectBuffer.objects[gl_InstanceIndex].model * objectBuffer.objects[gl_InstanceIndex].view * vec4(inPosition, 0.0, 1.0);
 fragColor = inColor;
 fragTexCoord = inTexCoord;
 
}