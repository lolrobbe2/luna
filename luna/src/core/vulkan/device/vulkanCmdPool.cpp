#include "vulkanCmdPool.h"
#include <core/debug/uuid.h>
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

		vulkanCmdPool::~vulkanCmdPool()
		{

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
				virtualCmdBuffer* currentBuffer = commandBuffer + i;
				*currentBuffer = uuid();
				virtualBuffers.insert({ *currentBuffer,{localCommandBuffers[i],0} });
			}
			return allocateResult;
		}
		VkResult vulkanCmdPool::begin(virtualCmdBuffer commandBuffer,const VkCommandBufferUsageFlags& usageFlags)
		{
			VkCommandBufferBeginInfo commandBufferBeginInfo = { VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO };
			commandBufferBeginInfo.flags = usageFlags;
			commandBufferBeginInfo.pInheritanceInfo = nullptr;
			commandBufferBeginInfo.pNext = nullptr;
			auto virtualBufferPair = virtualBuffers.find(commandBuffer);
			virtualBufferPair->second.second = usageFlags;
			vkResetCommandBuffer(virtualBufferPair->second.first, 0);
			//if ((virtualBufferPair->second.first == VK_NULL_HANDLE || (usageFlags & VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT)) != 0) { return VkResult::VK_ERROR_INITIALIZATION_FAILED; };
			

			return vkBeginCommandBuffer(virtualBufferPair->second.first, &commandBufferBeginInfo);
		}
		VkResult vulkanCmdPool::end(virtualCmdBuffer commandBuffer)
		{
			return vkEndCommandBuffer(virtualBuffers.find(commandBuffer)->second.first);
		}
		VkResult vulkanCmdPool::flush(VkQueue queue,uint64_t submitCount,const commandPoolSubmitInfo* pCommandPoolSubmitInfo,VkFence waitFence)
		{
			std::vector<VkSubmitInfo> submitInfos{};
			std::vector<VkCommandBuffer>commandBuffers;



			VkSubmitInfo submitInfo = { VK_STRUCTURE_TYPE_SUBMIT_INFO };
			for (size_t i = 0; i < pCommandPoolSubmitInfo->commandBufferCount; i++)
			{
				auto virtualBufferPair = virtualBuffers.find(*(pCommandPoolSubmitInfo->pCommandBuffers+i));
				commandBuffers.push_back(virtualBufferPair->second.first);
			}
			submitInfo.pCommandBuffers = commandBuffers.data();
			submitInfo.commandBufferCount = pCommandPoolSubmitInfo->commandBufferCount;
			submitInfo.pNext = pCommandPoolSubmitInfo->pNext;
			submitInfo.pSignalSemaphores = pCommandPoolSubmitInfo->pSignalSemaphores;
			submitInfo.pWaitDstStageMask = pCommandPoolSubmitInfo->pWaitDstStageMask;
			submitInfo.pWaitSemaphores = pCommandPoolSubmitInfo->pWaitSemaphores;
			submitInfo.signalSemaphoreCount = pCommandPoolSubmitInfo->signalSemaphoreCount;
			submitInfo.waitSemaphoreCount = pCommandPoolSubmitInfo->waitSemaphoreCount;
			VkResult result = vkQueueSubmit(queue, submitCount, &submitInfo, waitFence);
			return result;
		}
		void vulkanCmdPool::freeCommandBuffer(virtualCmdBuffer* pCommandBuffers, uint32_t count)
		{
			std::vector<VkCommandBuffer> commandBuffers;
			for (size_t i = 0; i < count; i++)
			{
				auto virtualBufferPair = virtualBuffers.find(*(pCommandBuffers + i));
				commandBuffers.push_back(virtualBufferPair->second.first);
			}
			vkFreeCommandBuffers(sCommandPoolSpec.device, commandPool, count,commandBuffers.data() );
		}
	}
}
