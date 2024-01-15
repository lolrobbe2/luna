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
		descriptorSet::descriptorSet(const VkDescriptorSet descriptorSet, const VkDescriptorPool* p_descriptorPool, std::vector<VkWriteDescriptorSet>& descriptorWrites) : _descriptorSet(descriptorSet),p_descriptorPool(p_descriptorPool),descriptorWrites(descriptorWrites)
		{
		}
	}
}


