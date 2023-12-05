#include "commandBuffer.h"
#include <core/debug/debugMacros.h>
namespace luna
{
	namespace artemis
	{
		commandBuffer::commandBuffer(const VkCommandPool* commandPool, const VkCommandBufferLevel& commandBufferLevel,const VkDevice* device)
		{
			VkCommandBufferAllocateInfo info{ VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO ,nullptr};
			info.commandBufferCount = 1;
			info.commandPool = *commandPool;
			info.level = commandBufferLevel;
			VkResult res = vkAllocateCommandBuffers(*device, &info, &m_commandBuffer);
			LN_ERR_FAIL_COND_MSG(res != VK_SUCCESS, "[Artemis] failed to allocate commandBuffer, VkResult: " + std::to_string(res));
			_commandPool = commandPool;
			_device = device;
		}
		bool commandBuffer::begin(const VkCommandBufferUsageFlags flags = VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT)
		{
			LN_ERR_FAIL_COND_MSG(recording, "you cannot call begin() on a commandBuffer that is allready recording");
			VkCommandBufferBeginInfo commandBufferBeginInfo = { VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO };
			commandBufferBeginInfo.flags = flags;
			commandBufferBeginInfo.pInheritanceInfo = nullptr;
			commandBufferBeginInfo.pNext = nullptr;
		
			vkResetCommandBuffer(m_commandBuffer, 0); //reset to make sure no chenanigans happens

			VkResult result = vkBeginCommandBuffer(m_commandBuffer, &commandBufferBeginInfo);
			LN_ERR_FAIL_COND_V_MSG(result != VK_SUCCESS, false, "could not start recording on commandBuffer, VkRsult: " + std::to_string(result));
			recording = true;
			return true;
		}
		void commandBuffer::end()
		{
			LN_ERR_FAIL_COND_MSG(!recording, "you cannot end recording a command buffer that has not begun recording! Consider using commandBuffer::begin function");
			VkResult result = vkEndCommandBuffer(m_commandBuffer);
			LN_ERR_FAIL_COND_MSG(result != VK_SUCCESS, "an error occured when stoping to record on the commandBuffer");
			recording = false;
		}
	}
}
