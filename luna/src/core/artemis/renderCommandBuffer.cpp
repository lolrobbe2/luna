#include "renderCommandBuffer.h"
#include <core/artemis/device/descriptorPool.h>
namespace luna
{
	namespace artemis 
	{
		renderCommandBuffer::renderCommandBuffer(const ref<allocator> p_allocator, descriptorPool& computePool, descriptorPool& graphicsPool)
		{
			cpuBuffer = p_allocator->allocateBuffer(sizeof(drawCommand) * LN_DRAW_COMMANDS_AMOUNT, CPU_TO_GPU, VK_BUFFER_USAGE_STORAGE_BUFFER_BIT);
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
		}
	}
}

