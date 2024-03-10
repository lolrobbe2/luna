#pragma once
#include <core/artemis/device/allocator.h>
#include <core/artemis/device/descriptorSet.h>
#define LN_DRAW_COMMANDS_AMOUNT 100
#define LN_IMAGE_BATCH_SIZE 32
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
        class descriptorPool;
        class renderCommandBuffer
        {
        public:
            renderCommandBuffer() = default;
            renderCommandBuffer(const ref<allocator> p_allocator,descriptorPool& computePool, descriptorPool& graphicsPool);
            bool addCommand(const drawCommand& command);
            void reset();
            void generateIndices();
            buffer& cpuIndicesBuffer = buffer();
            buffer& cpuBuffer = buffer(); //stores commands.
            buffer& gpuBuffer = buffer(); //stores vertices.
            descriptorSet& computeDescriptorSet = descriptorSet();
            descriptorSet& graphicsDescriptorSet = descriptorSet();
            drawCommand* p_commands = nullptr;
            drawCommand* p_commandsBase = nullptr;
            size_t commandsAmount = 0;
        };
    }
}


