#include "commandPool.h"
#include <core/debug/debugMacros.h>
namespace luna
{
	namespace artemis 
	{
		ref<commandBuffer> commandPool::getCommandBuffer()
		{
			return new commandBuffer();
		}
		commandPool::commandPool(const VkQueue queue, const uint32_t queueFamilyIndex,const VkCommandPoolCreateFlags flags,const VkDevice* device)
		{
			VkCommandPoolCreateInfo commandPoolCreateInfo{ VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO };
			commandPoolCreateInfo.pNext = nullptr;
			commandPoolCreateInfo.queueFamilyIndex = queueFamilyIndex;
			commandPoolCreateInfo.flags = flags;
			VkResult result = vkCreateCommandPool(*device, &commandPoolCreateInfo, nullptr, &m_commandPool);

			LN_ERR_FAIL_COND_MSG(result != VK_SUCCESS, "[artemis] an error ocuured during commandPool creation, VkResult code: " + std::to_string(result));
			this->queue = queue;
			this->device = device;
		}
	}
}