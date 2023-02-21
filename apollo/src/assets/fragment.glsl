#version 460
#extension GL_ARB_separate_shader_objects : enable
layout(location = 0) out vec4 outColor;
layout(location = 0) in vec4 fragColor;
layout(location = 1) in vec2 textCoord;
layout(location = 2) in float textIndex;
layout(set = 0,binding = 0) uniform sampler samp;
layout(set = 0,binding = 1) uniform texture2D textures[32];
void main() 
{   
    outColor = texture(sampler2D(textures[int(textIndex)], samp),textCoord) * fragColor;  
}