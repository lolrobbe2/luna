#include "commandBuffer.h"
namespace luna
{
	namespace artemis
	{
		commandBuffer::commandBuffer(const VkCommandPool commandPool, const VkCommandBufferLevel& commandBufferLevel)
		{
			VkCommandBufferAllocateInfo info{ VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO ,nullptr};

			info.commandBufferCount = 1;
			info.commandPool = commandPool;
			info.level = commandBufferLevel;
		}
	}
}
