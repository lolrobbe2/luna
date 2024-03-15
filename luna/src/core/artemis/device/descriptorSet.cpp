#include "descriptorSet.h"
#include <core/debug/debugMacros.h>
namespace luna
{
	namespace artemis 
	{
		void descriptorSet::free()
		{
			LN_ERR_FAIL_COND_MSG(p_device == nullptr || *p_device == VK_NULL_HANDLE, "[Artemis] could not free descriptorSet device handle was invalid (VK_NULL_HANDLE)");
			LN_ERR_FAIL_COND_MSG(p_descriptorPool == nullptr || *p_descriptorPool == VK_NULL_HANDLE, "[Artemis] the descriptorPool has already be destroyed and freeed the corresponding descriptorSets!");
			LN_ERR_FAIL_COND_MSG(_descriptorSet == VK_NULL_HANDLE, "[Artemis] descriptor set has already be destroyed");
			VkResult freeRes = vkFreeDescriptorSets(*p_device, *p_descriptorPool, 1, &_descriptorSet);

			LN_ERR_FAIL_COND_MSG(freeRes != VK_SUCCESS, "[Artemis] an error occured when freeing a descriptorSet, VkResult: " + std::to_string(freeRes));
			_descriptorSet = VK_NULL_HANDLE; 
		}
		descriptorSet::descriptorSet(const VkDevice* p_device,const VkDescriptorSet descriptorSet, const VkDescriptorPool* p_descriptorPool, std::vector<VkWriteDescriptorSet>& descriptorWrites) : p_device(p_device),_descriptorSet(descriptorSet),p_descriptorPool(p_descriptorPool),descriptorWrites(descriptorWrites)
		{
			for(VkWriteDescriptorSet& write : this->descriptorWrites)
			{
				write.dstSet = descriptorSet;
			}
		}
		void descriptorSet::update()
		{
			vkUpdateDescriptorSets(*p_device, descriptorWrites.size(), descriptorWrites.data(), 0, nullptr);
		}
		void descriptorSet::write(const uint32_t& descriptorIndex,const void* pDescriptorInfo)
		{
			LN_PROFILE_FUNCTION();
			switch (descriptorWrites[descriptorIndex].descriptorType)
			{
			case VK_DESCRIPTOR_TYPE_SAMPLER:
			case VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER:
			case VK_DESCRIPTOR_TYPE_SAMPLED_IMAGE:
			case VK_DESCRIPTOR_TYPE_STORAGE_IMAGE:
				descriptorWrites[descriptorIndex].pImageInfo = (const VkDescriptorImageInfo*)pDescriptorInfo;
				break;
			case VK_DESCRIPTOR_TYPE_UNIFORM_TEXEL_BUFFER:
			case VK_DESCRIPTOR_TYPE_STORAGE_TEXEL_BUFFER:
			case VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER:
			case VK_DESCRIPTOR_TYPE_STORAGE_BUFFER:
			case VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER_DYNAMIC:
			case VK_DESCRIPTOR_TYPE_STORAGE_BUFFER_DYNAMIC:
				descriptorWrites[descriptorIndex].pBufferInfo = (const VkDescriptorBufferInfo*)pDescriptorInfo;
				break;
			default:
				break;
			}
		}
	}
}


