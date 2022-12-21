#pragma once
#include <core/vulkan/window/window.h>
#include <core/vulkan/device/vulkanDevice.h>
namespace luna 
{
	namespace utils
	{
		/**
		 * @brief vulkan descriptor pool and descriptorset wrapper class.
		 */
		class vulkanDescriptorPool
		{
		public: 
			/**
			 * @brief descroptor pool constructor.
			 * 
			 */
			vulkanDescriptorPool(VkDescriptorPoolSize* pPoolSizes, const uint32_t& poolSizeCount);
			virtual ~vulkanDescriptorPool() = default;
			VkResult createDescriptorSet(VkDescriptorSet* descriptorSet);
			VkResult destroyDescriptorSet(VkDescriptorSet descriptorSet);
		private:
			ref<vulkan::vulkanDevice> device;
			VkDescriptorPool descriptorPool;
		};
	}
}


