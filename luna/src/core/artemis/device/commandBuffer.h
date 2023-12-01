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
			commandBuffer(const VkCommandPool commandPool, const VkCommandBufferLevel& commandBufferLevel);
		};
	}
}


