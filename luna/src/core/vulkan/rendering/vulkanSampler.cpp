#include "vulkanSampler.h"
namespace luna
{
	namespace renderer 
	{
		vulkanSampler::vulkanSampler(const ref<device>& device,const VkFilter& filters, const VkSamplerAddressMode& samplerAddressMode)
		{
			VkSamplerCreateInfo info = {};
			info.sType = VK_STRUCTURE_TYPE_SAMPLER_CREATE_INFO;
			info.pNext = nullptr;

			info.magFilter = filters;
			info.minFilter = filters;
			info.addressModeU = samplerAddressMode;
			info.addressModeV = samplerAddressMode;
			info.addressModeW = samplerAddressMode;
			vulkanDevice = std::dynamic_pointer_cast<vulkan::vulkanDevice>(vulkanDevice);
		
			vkCreateSampler(vulkanDevice->getDeviceHandles().device, &info, nullptr, &sampler);
		}
		vulkanSampler::~vulkanSampler()
		{
			vkDestroySampler(vulkanDevice->getDeviceHandles().device, sampler, nullptr);
		}
	}
}
