#include "vulkanCmdPool.h"
namespace luna
{
	namespace vulkan
	{
		vulkanCmdPool::vulkanCmdPool(const vulkanCmdBufferSpec& commandBufferSpec)
		{
			
			VkCommandPoolCreateInfo commandPoolCreateInfo{ VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO };
			commandPoolCreateInfo.pNext = nullptr;
			commandPoolCreateInfo.queueFamilyIndex = commandBufferSpec.queueFamilyIndex;
			vkCreateCommandPool(commandBufferSpec.device, &commandPoolCreateInfo, nullptr, &commandPool);
		}
		VkResult vulkanCmdPool::createNewBuffer(virtualCmdBuffer* commandBuffer)
		{
			
		}
		VkResult vulkanCmdPool::begin()
		{
			return VkResult();
		}
		VkResult vulkanCmdPool::end()
		{
			return VkResult();
		}
		VkResult vulkanCmdPool::flush()
		{
			return VkResult();
		}
	}
}
