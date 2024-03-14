#include "commandPool.h"
#include <core/debug/debugMacros.h>
#include <future>
#include <core/artemis/rendering/swapchain.h>
namespace luna
{
	namespace artemis 
	{
		void commandPool::_flush(VkQueue queue, const std::vector<commandBuffer*>& buffers, const std::vector<ref<semaphore>>& signalSemaphores, const std::vector<ref<semaphore>>& waitSemaphores, const ref<fence> fence, const VkPipelineStageFlags* pWaitDstStageMask, const bool separateThread)
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

			std::vector<VkSemaphore> nativeSignalSemaphores;
			for (auto& semaphorePtr : signalSemaphores) nativeSignalSemaphores.push_back(*semaphorePtr);
			
			submitInfo.pSignalSemaphores = nativeSignalSemaphores.data();


			//wait semaphore translation

			std::vector<VkSemaphore> nativeWaitSemaphores;
			for (auto& semaphorePtr : waitSemaphores) nativeWaitSemaphores.push_back(*semaphorePtr);
			
			submitInfo.pWaitSemaphores = nativeWaitSemaphores.data();

			if (separateThread) runnerMutex.lock();

			VkResult res = fence ? vkQueueSubmit(queue, 1, &submitInfo, *fence) : vkQueueSubmit(queue, 1, &submitInfo, nullptr);

			
			if (separateThread) { vkQueueWaitIdle(queue); runnerMutex.unlock(); }
			for (size_t i = 0; i < buffers.size(); i++) {
				buffers[i]->unlock();
			}
			LN_ERR_FAIL_COND_MSG(res != VK_SUCCESS, "[Artemis] unable to flush commandBuffers to queue");
		}
		
	    ref<commandBuffer> commandPool::getCommandBuffer(const VkCommandBufferLevel level) const
		{
			return ref<commandBuffer>(new commandBuffer(& m_commandPool, level, device));
		}
		void commandPool::flush(const std::vector<commandBuffer*>& buffers ,const std::vector<ref<semaphore>>& signalSemaphores,const std::vector<ref<semaphore>>& waitSemaphores,const ref<fence> fence,const VkPipelineStageFlags* pWaitDstStageMask,const bool separateThread) 
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
		VkResult commandPool::present(std::vector<ref<swapchain>> swapChains,std::vector<ref<semaphore>> waitSemaphores,uint32_t* pImageIndices)
		{
			std::vector<VkSwapchainKHR> nativeSwapchains;
			std::vector<VkSemaphore> nativeSemaphores;

			nativeSwapchains.resize(swapChains.size(), *swapChains[nativeSwapchains.size()]);
			nativeSemaphores.resize(waitSemaphores.size(), *waitSemaphores[nativeSemaphores.size()]);

			VkPresentInfoKHR presentInfo = {};
			presentInfo.sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR;
			presentInfo.pNext = nullptr;

			presentInfo.pSwapchains = nativeSwapchains.data();
			presentInfo.swapchainCount = nativeSwapchains.size();
			presentInfo.pWaitSemaphores = nativeSemaphores.data();
			presentInfo.waitSemaphoreCount = nativeSemaphores.size();

			presentInfo.pImageIndices = pImageIndices;
			return vkQueuePresentKHR(queue, &presentInfo);
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