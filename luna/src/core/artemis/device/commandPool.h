#pragma once
#include <core/platform/windows/windowsWindow.h>
#include <core/artemis/device/commandBuffer.h>
namespace luna
{
	namespace artemis 
	{
		class commandPool
		{
			ref<commandBuffer> getCommandBuffer();
		private:
			VkCommandPool m_commandPool = VK_NULL_HANDLE;
			std::thread runner; 
			std::mutex runnerMutex;
			VkQueue queue = VK_NULL_HANDLE;
			const VkDevice* device;
		protected:
			friend class device;
			commandPool(const VkQueue queue,const uint32_t queueFamilyIndex, const VkCommandPoolCreateFlags flags, const VkDevice* device);
		};
	}
}


