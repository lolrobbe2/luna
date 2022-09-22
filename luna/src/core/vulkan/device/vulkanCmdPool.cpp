#include "vulkanCmdPool.h"
namespace luna
{
	namespace vulkan
	{
		vulkanCmdPool::vulkanCmdPool(const vulkanCmdPoolSpec& commandPoolSpec)
		{
			sCommandPoolSpec = commandPoolSpec;
			VkCommandPoolCreateInfo commandPoolCreateInfo{ VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO };
			commandPoolCreateInfo.pNext = nullptr;
			commandPoolCreateInfo.queueFamilyIndex = commandPoolSpec.queueFamilyIndex;
			commandPoolCreateInfo.flags = commandPoolSpec.flags;
			vkCreateCommandPool(commandPoolSpec.device, &commandPoolCreateInfo, nullptr, &commandPool);
		}
		VkResult vulkanCmdPool::createNewBuffer( virtualCmdBuffer* commandBuffer,const uint32_t& commandBufferCount,const VkCommandBufferLevel& commandBufferLevel)
		{
			VkCommandBufferAllocateInfo allocateInfo{ VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO };
			allocateInfo.commandBufferCount =  commandBufferCount;
			allocateInfo.level = commandBufferLevel;
			allocateInfo.commandPool = commandPool;
			std::vector<VkCommandBuffer> localCommandBuffers(commandBufferCount);
			VkResult allocateResult = vkAllocateCommandBuffers(sCommandPoolSpec.device, &allocateInfo, localCommandBuffers.data());
			for (size_t i = 0; i < commandBufferCount; i++)
			{
				virtualCmdBuffer* currentBuffer = commandBuffer + commandBufferCount;
				*currentBuffer = uuid();
				virtualBuffers.insert({ *currentBuffer,{localCommandBuffers[i],0} });
				LN_CORE_INFO("added virtualbuffer, uuid = {0} ,adress = {1}", *currentBuffer, currentBuffer);
			}
			return allocateResult;
		}
		VkResult vulkanCmdPool::begin(virtualCmdBuffer commandBuffer,VkCommandBufferUsageFlags usageFlags)
		{
			VkCommandBufferBeginInfo commandBufferBeginInfo = { VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO };
			commandBufferBeginInfo.flags = usageFlags;
			commandBufferBeginInfo.pInheritanceInfo = nullptr;
			commandBufferBeginInfo.pNext = nullptr;
			auto virtualBufferPair = virtualBuffers.find(commandBuffer);
			virtualBufferPair->second.second = usageFlags;
			if (virtualBufferPair->second.first == VK_NULL_HANDLE || usageFlags & VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT != 0) { return VkResult::VK_ERROR_INITIALIZATION_FAILED; };
			return vkBeginCommandBuffer(virtualBufferPair->second.first, &commandBufferBeginInfo);
		}
		VkResult vulkanCmdPool::end(virtualCmdBuffer commandBuffer)
		{
			vkEndCommandBuffer(virtualBuffers.find(commandBuffer)->second.first);
		}
		VkResult vulkanCmdPool::flush()
		{
			VkSubmitInfo submitInfo = { VK_STRUCTURE_TYPE_SUBMIT_INFO };
			//TODO
			vkQueueSubmit()
		}
	}
}
