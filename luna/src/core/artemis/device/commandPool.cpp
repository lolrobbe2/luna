#include "commandPool.h"
#include <core/debug/debugMacros.h>
#include <future>
namespace luna
{
	namespace artemis 
	{
		void commandPool::_flush(VkQueue queue, const std::vector<commandBuffer*>& buffers, const std::vector<semaphore>& signalSemaphores, const std::vector<semaphore>& waitSemaphores, const fence& fence, const VkPipelineStageFlags* pWaitDstStageMask, const bool separateThread)
		{
			LN_PROFILE_FUNCTION();
			VkSubmitInfo submitInfo{ VK_STRUCTURE_TYPE_SUBMIT_INFO };
			submitInfo.commandBufferCount = buffers.size();
			submitInfo.signalSemaphoreCount = signalSemaphores.size();
			submitInfo.waitSemaphoreCount = waitSemaphores.size();
			submitInfo.pWaitDstStageMask = pWaitDstStageMask;
			//commandBuffer transformation

			std::vector<VkCommandBuffer> commandBuffers;
			for (size_t i = 0; i < buffers.size(); i++) {
				commandBuffers.push_back(*buffers[i]);
				buffers[i]->lock();
			}

			submitInfo.pCommandBuffers = commandBuffers.data();

			//signal semaphore translation

			VkSemaphore* pSignalSemaphores = new VkSemaphore[signalSemaphores.size()];
			for (size_t i = 0; i < signalSemaphores.size(); i++) pSignalSemaphores[i] = signalSemaphores[i];

			submitInfo.pSignalSemaphores = pSignalSemaphores;


			//wait semaphore translation

			VkSemaphore* pWaitSemaphores = new VkSemaphore[waitSemaphores.size()];
			for (size_t i = 0; i < waitSemaphores.size(); i++) pWaitSemaphores[i] = waitSemaphores[i];

			submitInfo.pWaitSemaphores = pWaitSemaphores;
			if (separateThread) runnerMutex.lock();

			VkResult res = vkQueueSubmit(queue, 1, &submitInfo, fence);

			
			if (separateThread) { vkQueueWaitIdle(queue); runnerMutex.unlock(); }
			for (size_t i = 0; i < buffers.size(); i++) {
				buffers[i]->lock();
			}
			LN_ERR_FAIL_COND_MSG(res != VK_SUCCESS, "[Artemis] unable to flush commandBuffers to queue");
		}
		
	    ref<commandBuffer> commandPool::getCommandBuffer(const VkCommandBufferLevel level) const
		{
			return ref<commandBuffer>(new commandBuffer(& m_commandPool, level, device));
		}
		void commandPool::flush(const std::vector<commandBuffer*>& buffers ,const std::vector<semaphore>& signalSemaphores,const std::vector<semaphore>& waitSemaphores,const fence& fence,const VkPipelineStageFlags* pWaitDstStageMask,const bool separateThread) 
		{
			LN_PROFILE_FUNCTION();
			if(separateThread) 
			{
				
				std::async(std::launch::async, [&] {
					_flush(queue, buffers, signalSemaphores, waitSemaphores, fence, pWaitDstStageMask, separateThread);
					});
			}
			else 
			{
				_flush(queue, buffers, signalSemaphores, waitSemaphores, fence, pWaitDstStageMask, separateThread);
			}
		}
		commandPool::commandPool(const VkQueue queue, const uint32_t queueFamilyIndex,const VkCommandPoolCreateFlags flags,const VkDevice* device)
		{
			VkCommandPoolCreateInfo commandPoolCreateInfo{ VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO };
			commandPoolCreateInfo.pNext = nullptr;
			commandPoolCreateInfo.queueFamilyIndex = queueFamilyIndex;
			commandPoolCreateInfo.flags = flags | VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT;
			VkResult result = vkCreateCommandPool(*device, &commandPoolCreateInfo, nullptr, &m_commandPool);

			LN_ERR_FAIL_COND_MSG(result != VK_SUCCESS, "[artemis] an error ocuured during commandPool creation, VkResult code: " + std::to_string(result));
			this->queue = queue;
			this->device = device;
		}
	}
}