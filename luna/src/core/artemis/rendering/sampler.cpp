#include "sampler.h"
#include <core/debug/debugMacros.h>
namespace luna 
{
	namespace artemis 
	{
		sampler::sampler(const VkDevice* p_device, const VkFilter& filters, const VkSamplerAddressMode& samplerAddressMode) : p_device(p_device)
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
			VkResult createRes = vkCreateSampler(*p_device, &info, nullptr, &_sampler);
			LN_ERR_FAIL_COND_MSG(createRes != VK_SUCCESS, "[Artemis] an error occured when creating a smapler, VkResult: " + VK_RESULT(createRes));
		
		}
		sampler::~sampler()
		{
			LN_ERR_FAIL_COND_MSG(*p_device == VK_NULL_HANDLE, "[Artemis] could not destroy sampler because *p_device was VK_NULL_HANDLE!");
			LN_ERR_FAIL_COND_MSG(_sampler == VK_NULL_HANDLE, "[Artemis] could not destroy sampler because it has already been destroyed!");	
			vkDestroySampler(*p_device, _sampler, nullptr);
			_sampler = VK_NULL_HANDLE; 
		}
	}
}