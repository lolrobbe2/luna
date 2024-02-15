#pragma once
#include <core/artemis/device/allocator.h>
#define LN_DRAW_COMMANDS_AMOUNT 100
namespace luna 
{
    namespace artemis 
    {
        typedef struct drawCommand
        {
            glm::mat4 transform;
            glm::vec4 color;
            glm::vec2 textureCoords[4] = { { 0.0f, 0.0f }, { 1.0f, 0.0f }, { 1.0f, 1.0f }, { 0.0f, 1.0f } };
            float textureIndex = 0;
            float text = false;
        } drawCommand;
        class renderCommandBuffer
        {
        public:
            renderCommandBuffer() = default;
            renderCommandBuffer(const ref<allocator> p_allocator);
            bool addCommand(const drawCommand& command);
            void reset();
            buffer& cpuBuffer = buffer(); //stores commands.
            buffer& gpuBuffer = buffer(); //stores vertices.
            drawCommand* p_commands;
            drawCommand* p_commandsBase;
            size_t commandsAmount;
        };
    }
}


