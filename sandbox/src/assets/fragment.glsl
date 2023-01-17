#version 460
#extension GL_ARB_shader_draw_parameters : enable

layout(location = 0) out vec4 outColor;
layout(location = 0) in vec4 fragColor;
layout(location = 1) in vec3 textCoord;
layout(set = 0,binding = 0) uniform sampler samp;
layout(set = 0,binding = 1) uniform texture2D textures[32];

void main() 
{
	outColor = fragColor * texture(sampler2D(textures[int(textCoord.z)], samp),vec2(textCoord.x,textCoord.y));
}