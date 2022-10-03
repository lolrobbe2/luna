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
	mat4 model;
    mat4 view;
    mat4 proj;
    bool text;
};




layout(location = 0) out vec4 outColor;
layout(location = 1) in vec2 fragTexCoord;
layout(location = 0) in vec3 fragColor;
layout(set = 0,binding = 1) uniform sampler samp;
layout(set = 0,binding = 2) uniform texture2D textures[32];
layout(std140,set = 0, binding = 3) buffer ObjectBuffer
{
	ObjectData objects[];
} objectBuffer;

void main() 
{
	//vec4 textureColor = texture(texSampler[push.index], fragTexCoord);
	if(push.text == 1)
	{
		outColor = vec4(fragColor.x,fragColor.y,fragColor.z,texture(sampler2D(textures[push.index], samp),fragTexCoord).r);   
	}
	else
	{
		outColor = texture(sampler2D(textures[push.index], samp),fragTexCoord);
	}


}