#include "vulkanSampler.h"
namespace luna
{
	namespace renderer 
	{
		vulkanSampler::vulkanSampler(ref<vulkan::vulkanDevice> device,const VkFilter& filters, const VkSamplerAddressMode& samplerAddressMode)
		{
			VkSamplerCreateInfo info = {};
			info.sType = VK_STRUCTURE_TYPE_SAMPLER_CREATE_INFO;
			info.pNext = nullptr;

			info.magFilter = filters;
			info.minFilter = filters;
			info.addressModeU = samplerAddressMode;
			info.addressModeV = samplerAddressMode;
			info.addressModeW = samplerAddressMode;
		
			vkCreateSampler(device->getDeviceHandles().device, &info, nullptr, &sampler);
			vulkanDevice = device;
		}
		vulkanSampler::~vulkanSampler()
		{
			vkDestroySampler(vulkanDevice->getDeviceHandles().device, sampler, nullptr);
		}
	}
}
