#pragma once
#include <core/platform/windows/windowsWindow.h>
namespace luna
{
	namespace artemis 
	{
		class commandPool
		{
			/**
			 * @brief starts recording a commandBuffer.
			 * @param virtualCommandBuffer commandBuffer handle.
			 * @param const VkCommandBufferUsageFlags& usageFlags
			 */
			VkResult begin(const VkCommandBuffer buffer, const VkCommandBufferUsageFlags& usageFlags);
			/**
			* @brief stops recording the commandBuffer.
			* @param virtualCommandBuffer commandBuffer handle.
			*/
			VkResult end(const VkCommandBuffer buffer);
		private:
			VkCommandPool commandPool;
			std::thread runner;
		};
	}
}


