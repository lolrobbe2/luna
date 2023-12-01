#pragma once
#include <core/vulkan/window/window.h>

namespace luna 
{
	namespace artemis 
	{
		class commandBuffer
		{
		protected: 
			friend class commandPool;
			commandBuffer(const VkCommandPool commandPool, const VkCommandBufferLevel& commandBufferLevel,const VkDevice device);
			bool begin(const VkCommandBufferUsageFlags flags);
			void end();
			bool isRecording() { return recording; }
		private:
			bool recording = false;
			VkCommandBuffer m_commandBuffer;
		};
	}
}


