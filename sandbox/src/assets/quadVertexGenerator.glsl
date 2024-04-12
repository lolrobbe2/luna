#version 460
#pragma vscode_glsllint_stage : frag

/**
@brief this shaders calcultes the the vertices based on a given draw command.
@brief these calculations used to be done on the cpu but can be done on the GPU and thus improve performance!
*/

//set workgroup size
layout (local_size_x = 512, local_size_y = 1, local_size_z = 1) in;

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

// IO structs
struct drawCommand
{
    mat4 transform;
    vec4 color;
    vec2 textureCoords[4];
 	vec2 textureDetails;
};

struct quadVertex
{
	vec4 vert;
    vec4 color;
	vec2 textureCoord;
    float textureIndex;
    float text;
};

layout(binding = 0) readonly buffer drawCommandsSSBO {
    drawCommand drawCommandsIn[ ];
};
layout(binding = 1) buffer vertexSSBO
{
    quadVertex verticesOut[ ];
};

//consts
const uint quadVertexCount = 4;
const uint outIndexMultiplier = 4;



void main()
{

     uint localInvocationID = gl_LocalInvocationID.x;

    // Calculate the index of the SSBO element to access
    uint index = localInvocationID + gl_WorkGroupSize.x * gl_WorkGroupID.x;
    uint outIndex = index * outIndexMultiplier; //index is multiplied by 4 to get the correct quad offset in memory. because quads have 4 quadVertices

    for (int subIndex = 0; subIndex < quadVertexCount; subIndex++)
    {
        //vertexPositions are different for text and quads.
        if(drawCommandsIn[index].textureDetails.y > 0.0f) verticesOut[outIndex + subIndex].vert = drawCommandsIn[index].transform * quadCharVertexPositions[subIndex];
        else verticesOut[outIndex + subIndex].vert = drawCommandsIn[index].transform * quadCharVertexPositions[subIndex];

        verticesOut[outIndex + subIndex].textureIndex = drawCommandsIn[index].textureDetails.x;
        verticesOut[outIndex + subIndex].color = drawCommandsIn[index].color;
        verticesOut[outIndex + subIndex].text = drawCommandsIn[index].textureDetails.y;
        verticesOut[outIndex + subIndex].textureCoord = drawCommandsIn[index].textureCoords[subIndex];
    }
}
