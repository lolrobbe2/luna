#version 460
#extension GL_ARB_shader_draw_parameters : enable




layout(location = 0) out vec4 outColor;



void main() 
{
	//vec4 textureColor = texture(texSampler[push.index], fragTexCoord);
	outColor = vec4(255.0f,0.0f,0.0f,50.0f);


}