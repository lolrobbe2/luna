#pragma once
#include <core/artemis/device/commandBuffer.h>
#include <core/artemis/device/semaphore.h>
#include <core/artemis/device/semaphore.h>
#include <core/artemis/device/fence.h>
namespace luna
{
	namespace artemis 
	{
		class commandPool
		{
		public:
			ref<commandBuffer> getCommandBuffer(const VkCommandBufferLevel level = VK_COMMAND_BUFFER_LEVEL_PRIMARY) const;
			void flush(const std::vector<commandBuffer>& buffers, const std::vector<semaphore>& signalSemaphores, const std::vector<semaphore>& waitSemaphores, const fence& fence, const VkPipelineStageFlags* pWaitDstStageMask, const bool separateThread);
			~commandPool() { vkDestroyCommandPool(*device, m_commandPool, nullptr); }
		private:
			void _flush(VkQueue queue, const std::vector<commandBuffer>& buffers, const std::vector<semaphore>& signalSemaphores, const std::vector<semaphore>& waitSemaphores, const fence& fence, const VkPipelineStageFlags* pWaitDstStageMask, const bool separateThread);
			VkCommandPool m_commandPool = VK_NULL_HANDLE;
			std::thread runner; 
		    mutable std::mutex runnerMutex;
			VkQueue queue = VK_NULL_HANDLE;
			const VkDevice* device;
		protected:
			friend class device;
			commandPool(const VkQueue queue,const uint32_t queueFamilyIndex, const VkCommandPoolCreateFlags flags, const VkDevice* device);
		};
	}
}


