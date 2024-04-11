#pragma once
#include <core/artemis/device/allocator.h>
#include <core/artemis/device/descriptorSet.h>
#define LN_DRAW_COMMANDS_AMOUNT 10000
#define LN_IMAGE_BATCH_SIZE 32
namespace luna 
{
    namespace assets {
        class image;
    }
    namespace artemis 
    {
        typedef struct drawCommand
        {
            glm::mat4 transform;
            glm::vec4 color = { 1.0f,1.0f,1.0f,1.0f };
            glm::vec2 textureCoords[4] = { { 0.0f, 0.0f }, { 1.0f, 0.0f }, { 1.0f, 1.0f }, { 0.0f, 1.0f } };
            float textureIndex = 0;
            float text = false;
        } drawCommand;
        class descriptorPool;
        class sampler;
        typedef struct quadVertex
        {
            glm::vec4 vert;
            glm::vec4 color;
            glm::vec2 textureCoord;
            float textureIndex;
            float text;
        };
        class renderCommandBuffer
        {
        public:
            renderCommandBuffer() = default;
            renderCommandBuffer(const ref<allocator> p_allocator,descriptorPool& computePool, descriptorPool& graphicsPool,ref<sampler> sampler,uint8_t maxFramesInflight);
            bool addCommand(const drawCommand& command);
            void reset();
            void generateIndices();
            /// <summary>
            /// Tries to bind an image to the current descriptor set.
            /// </summary>
            /// <param name="image">Image to bind</param>
            /// <param name="currentDescriptorSetIndex">Descriptor Set index to bind to</param>
            /// <param name="currentFrame">Current frame that is recording</param>
            /// <returns>
            ///     <para>
            ///         - True if the image was successfully bound.
            ///     </para>
            ///     <para>
            ///         - False if the image binding failed (no more freeImageIndices).
            ///     </para>
            /// </returns>
            /// does not need currentFrame see update func for reason why!
            bool bind(ref<assets::image> image, uint32_t currentDescriptorSetIndex);
            void unbind(uint8_t index);
            void update(uint8_t currentFrame){
                graphicsDescriptorSets[currentFrame].write(0, &samplerInfo);
                graphicsDescriptorSets[currentFrame].update();
            }
            buffer& cpuIndicesBuffer = *new buffer();
            buffer& cpuBuffer = *new buffer(); //stores commands.
            buffer& gpuBuffer = *new buffer(); //stores vertices.
            descriptorSet& computeDescriptorSet = *new descriptorSet();
            std::vector<descriptorSet> graphicsDescriptorSets;
            std::vector<uint8_t> freeImageIndeces;
            ref<assets::image> images[LN_IMAGE_BATCH_SIZE];
            drawCommand* p_commands = nullptr;
            drawCommand* p_commandsBase = nullptr; 
            VkDescriptorImageInfo samplerInfo;
            std::vector<VkDescriptorImageInfo> descriptorInfos{ LN_IMAGE_BATCH_SIZE };
            size_t commandsAmount = 0;
        };
    }
}


