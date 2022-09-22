#pragma once
#include <core/vulkan/window/window.h>
namespace luna
{
	namespace vulkan
	{
		
		typedef uint64_t virtualCmdBuffer;
		struct vulkanCmdPoolSpec
		{
			VkDevice device;
			VkCommandPoolCreateFlags    flags;
			uint32_t queueFamilyIndex;
		};
		class vulkanCmdPool
		{
		public:
			VkCommandBuffer operator=(const virtualCmdBuffer commandBuffer) { return virtualBuffers.find(commandBuffer)->second.first; };
			vulkanCmdPool(const vulkanCmdPoolSpec& commandBufferSPec);
			virtual ~vulkanCmdPool(){};
			VkResult createNewBuffer(virtualCmdBuffer* commandBuffer, const uint32_t& commandBufferCount = 1, const VkCommandBufferLevel& commandBufferLevel = VK_COMMAND_BUFFER_LEVEL_PRIMARY);
			VkResult begin(virtualCmdBuffer commandBuffer, VkCommandBufferUsageFlags usageFlags);
			VkResult end(virtualCmdBuffer commandBuffer);
			VkResult flush();
		private:
			std::unordered_map<virtualCmdBuffer, std::pair<VkCommandBuffer, VkCommandBufferUsageFlags>> virtualBuffers;
			VkCommandPool commandPool;
			vulkanCmdPoolSpec sCommandPoolSpec;
		};

	}
}


