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
			info.mipmapMode = filters == VK_FILTER_LINEAR ? VK_SAMPLER_MIPMAP_MODE_LINEAR : VK_SAMPLER_MIPMAP_MODE_NEAREST;
			info.compareEnable = VK_FALSE;
			info.compareOp = VK_COMPARE_OP_ALWAYS;
			info.mipLodBias = 0.0f;
			info.minLod = 0.0f;
			info.maxLod = 0.0f;
			vkCreateSampler(device->getDeviceHandles().device, &info, nullptr, &sampler);
			vulkanDevice = device;
		}
		vulkanSampler::~vulkanSampler()
		{
			vkDestroySampler(vulkanDevice->getDeviceHandles().device, sampler, nullptr);
		}
	}
}
