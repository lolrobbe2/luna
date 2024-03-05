#include "renderCommandBuffer.h"
#include <core/artemis/device/descriptorPool.h>
namespace luna
{
	namespace artemis 
	{
		renderCommandBuffer::renderCommandBuffer(const ref<allocator> p_allocator, descriptorPool& computePool, descriptorPool& graphicsPool)
		{
			cpuBuffer = p_allocator->allocateBuffer(sizeof(drawCommand) * LN_DRAW_COMMANDS_AMOUNT, CPU_TO_GPU, VK_BUFFER_USAGE_STORAGE_BUFFER_BIT);
			cpuIndicesBuffer = p_allocator->allocateBuffer(LN_DRAW_COMMANDS_AMOUNT * 6, CPU_TO_GPU, VK_BUFFER_USAGE_STORAGE_BUFFER_BIT);
			gpuBuffer = p_allocator->allocateBuffer(cpuBuffer.getSize() * 4, GPU_ONLY, VK_BUFFER_USAGE_STORAGE_BUFFER_BIT | VK_BUFFER_USAGE_VERTEX_BUFFER_BIT);
			p_commandsBase = (drawCommand*) cpuBuffer.getData();
			p_commands = p_commandsBase;

			computeDescriptorSet = computePool.allocateDescriptorSet();
			graphicsDescriptorSet = graphicsPool.allocateDescriptorSet();

			VkDescriptorBufferInfo info;
			info.buffer = cpuBuffer;
			info.offset = 0;
			info.range = VK_WHOLE_SIZE;

			VkDescriptorBufferInfo vertexInfo;
			vertexInfo.buffer = cpuBuffer;
			vertexInfo.offset = 0;
			vertexInfo.range = VK_WHOLE_SIZE;
			computeDescriptorSet.write(0, &info);
			computeDescriptorSet.write(1, &vertexInfo);
			computeDescriptorSet.update();

			//graphicsDescriptorSet.write(0,)
		}
		bool renderCommandBuffer::addCommand(const drawCommand& command)
		{
			if (commandsAmount < LN_DRAW_COMMANDS_AMOUNT) {
				*p_commands = command;
				p_commands++;
				commandsAmount++;
				return false;
			}
			return true; //buffer full create new buffer;
		}
		void renderCommandBuffer::reset()
		{
			p_commands = p_commandsBase;
			commandsAmount = 0;
		}
		void renderCommandBuffer::generateIndices()
		{
			uint32_t offset = 0;
			uint32_t* quadIndices = cpuIndicesBuffer.getData<uint32_t>();
			//*2 because a rectangle/quad exists out of 2 triangles.
			for (uint32_t i = 0; i < commandsAmount * 2; i += 6)
			{
				quadIndices[i + 0] = offset + 0;
				quadIndices[i + 1] = offset + 1;
				quadIndices[i + 2] = offset + 2;

				quadIndices[i + 3] = offset + 2;
				quadIndices[i + 4] = offset + 3;
				quadIndices[i + 5] = offset + 0;

				offset += 4;
			}
		}
	}
}

