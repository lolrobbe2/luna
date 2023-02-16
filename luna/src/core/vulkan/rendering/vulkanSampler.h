#pragma once
#include <core/vulkan/device/vulkanDevice.h>
namespace luna
{
	namespace renderer
	{
		/**
		 * @brief vulkan sampler wrapper class to be superceded by multiplatform implementation later.
		 */
		class vulkanSampler
		{
		public:
			/**
			 * @brief sampler constructor.
			 * 
			 * \param device reference
			 * \param filters
			 * \param samplerAddressMode
			 */
			vulkanSampler(ref<vulkan::vulkanDevice> device,const VkFilter& filters, const VkSamplerAddressMode& samplerAddressMode = VK_SAMPLER_ADDRESS_MODE_REPEAT);
			~vulkanSampler();
			/**
			 * @brief returns the vulkan native sampler handle.
			 * @returns VkSampler handle
			 */
			inline VkSampler getHandle() { return sampler; };
		private:
			VkSampler sampler = VK_NULL_HANDLE;
			ref<vulkan::vulkanDevice> vulkanDevice; //local ref to device obj.
		};
	}
}


