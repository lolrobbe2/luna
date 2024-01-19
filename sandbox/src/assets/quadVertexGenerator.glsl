#version 460
#pragma vscode_glsllint_stage : frag

/**
@brief this shaders calcultes the the vertices based on a given draw command.
@brief these calculations used to be done on the cpu but can be done on the GPU and thus improve performance!
*/

//vertex positions.
const vec4 quadVertexPositions[4] = {
    { -0.5f, -0.5f, 0.0f, 1.0f },
    { 0.5f, -0.5f, 0.0f, 1.0f },
    { 0.5f,  0.5f, 0.0f, 1.0f },
    { -0.5f,  0.5f, 0.0f, 1.0f }
    };
const vec4 quadCharVertexPositions[4] = {
    { 0.0f, 0.0f, 0.0f, 1.0f },
    { 1.0f, 0.0f, 0.0f, 1.0f },
    { 1.0f,  1.0f, 0.0f, 1.0f },
    { 0.0f,  1.0f, 0.0f, 1.0f }
};


struct drawCommand
{
    mat4 transform;
    vec4 color;
    vec2 textureCoords[4];
    float textureIndex;
    float text;
};

struct quadVertex
{
	vec4 vert;
    vec4 color;
	vec2 textureCoord;
	float textureIndex;
	float text;
};
const int quadVertexCount = 4;
const int outIndexMultiplier = quadVertex * 4;
layout(std140, binding = 0) readonly buffer drawCommandsSSBO {
    drawCommand drawCommandsIn[ ];
};
layout(std140, binding = 1) buffer vertexSSBO
{
    quadVertex verticesOut[ ];
}
//set workgroup size
layout (local_size_x = 16, local_size_y = 16, local_size_z = 1) in;

void main()
{
    uint index = gl_GlobalInvocationID.x;
    for (int i = 0; i < quadVertexCount; i++)
    {
        int outIndex = index * outIndexMultiplier; //index is multiplied by for to get the correct quad offset in memory. because quads have 4 quadVertices
        //vertexPositions are different for text and quads.
        if(text) verticesOut[outIndex + i].vert = drawCommand[index].transform * quadCharVertexPositions[i];
        else verticesOut[outIndex + i].vert = drawCommand[index] * quadCharVertexPositions[i];    }

        verticesOut[outIndex + i].textureIndex = drawCommand[index].textureIndex;
        verticesOut[outIndex + i].color = drawCommand[index].color;
        verticesOut[outIndex + i].text = drawCommand[index].text;
        verticesOut[outIndex + i].textureCoord = drawCommand[index].textureCoords[i];
    }
}
