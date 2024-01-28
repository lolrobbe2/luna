#include "renderCommandBuffer.h"
namespace luna
{
	namespace artemis 
	{
		renderCommandBuffer::renderCommandBuffer(const ref<allocator> p_allocator)
		{
			cpuBuffer = p_allocator->allocateBuffer(sizeof(drawCommand) * LN_DRAW_COMMANDS_AMOUNT, CPU_TO_GPU, VK_BUFFER_USAGE_STORAGE_BUFFER_BIT);
			gpuBuffer = p_allocator->allocateBuffer(cpuBuffer.getSize() * 4, GPU_ONLY, VK_BUFFER_USAGE_STORAGE_BUFFER_BIT | VK_BUFFER_USAGE_VERTEX_BUFFER_BIT);
			p_commandsBase = (drawCommand*) cpuBuffer.getData();
			p_commands = p_commandsBase;
		}
		bool renderCommandBuffer::addCommand(const drawCommand& command)
		{
			if (commandsAmount < LN_DRAW_COMMANDS_AMOUNT) {
				*p_commands = command;
				p_commands++;
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

