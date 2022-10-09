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
				uint64_t uid = uuid();
				*currentBuffer = uid;
				virtualBuffers.insert({ uid,{localCommandBuffers[i],0} });
				LN_CORE_INFO("added virtualbuffer, uuid = {0} ", *currentBuffer);
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
			
			VkSubmitInfo submitInfo = { VK_STRUCTURE_TYPE_SUBMIT_INFO };
			commandBuffers.resize(0);
			for (size_t i = 0; i < pCommandPoolSubmitInfo->commandBufferCount; i++)
			{
				auto virtualBufferPair = virtualBuffers.find(*(pCommandPoolSubmitInfo->pCommandBuffers+i));
				if (virtualBufferPair->second.second == VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT) vkResetCommandBuffer(virtualBufferPair->second.first,0);
				commandBuffers.push_back(virtualBufferPair->second.first);
			}
			submitInfo.pCommandBuffers = commandBuffers.data();
			submitInfo.commandBufferCount = commandBuffers.size();
			submitInfo.pNext = pCommandPoolSubmitInfo->pNext;
			submitInfo.pSignalSemaphores = pCommandPoolSubmitInfo->pSignalSemaphores;
			submitInfo.pWaitDstStageMask = pCommandPoolSubmitInfo->pWaitDstStageMask;
			submitInfo.pWaitSemaphores = pCommandPoolSubmitInfo->pWaitSemaphores;
			submitInfo.signalSemaphoreCount = pCommandPoolSubmitInfo->signalSemaphoreCount;
			submitInfo.waitSemaphoreCount = pCommandPoolSubmitInfo->waitSemaphoreCount;
			
			return vkQueueSubmit(queue, submitCount, &submitInfo, waitFence);
		}
	}
}
