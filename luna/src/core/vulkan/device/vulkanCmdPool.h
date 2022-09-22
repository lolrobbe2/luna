#pragma once
#include <core/vulkan/window/window.h>
namespace luna
{
	namespace vulkan
	{
		typedef uint64_t virtualCmdBuffer;
		struct vulkanCmdBufferSpec
		{
			VkDevice device;
			VkCommandPoolCreateFlags    flags;
			uint32_t queueFamilyIndex;
		};
		class vulkanCmdPool
		{
		public:
			vulkanCmdPool(const vulkanCmdBufferSpec& commandBufferSPec);
			virtual ~vulkanCmdPool(){};
			VkResult createNewBuffer(virtualCmdBuffer* commandBuffer);
			VkResult begin();
			VkResult end();
			VkResult flush();
		private:
			std::unordered_map<virtualCmdBuffer, VkCommandBuffer> virtualBuffers;
			VkCommandPool commandPool;
		};

	}
}


