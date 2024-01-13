#include "descriptorPool.h"
#include <core/debug/debugMacros.h>
namespace luna 
{
	namespace artemis 
	{
		descriptorPool::descriptorPool(const VkDevice* device,VkDescriptorPoolCreateInfo info, std::vector<VkWriteDescriptorSet>& descriptorWrites)
		{
			VkDescriptorPool tempDescriptorPool; //temp used to make sure that when an error occured _desscriptorPool stays VK_NULL_HANDLE
			VkResult createResult = vkCreateDescriptorPool(*device, &info, nullptr, &tempDescriptorPool);
			LN_ERR_FAIL_COND_MSG(createResult != VK_SUCCESS, "[Artemis] unable to create descriptor set, VkResult:  " + std::to_string(createResult));
			p_device = device;
			_descriptorPool = tempDescriptorPool;
			this->descriptorWrites = descriptorWrites;
		}
		descriptorSet descriptorPool::allocateDescriptorSet()
		{
			VkDescriptorSetAllocateInfo allocateInfo{ VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO };
			allocateInfo.descriptorPool = _descriptorPool;
			allocateInfo.descriptorSetCount = 1;
			allocateInfo.pSetLayouts = &layout;
			VkDescriptorSet set;
			vkAllocateDescriptorSets(*p_device, &allocateInfo, &set);
			return descriptorSet(set,&_descriptorPool,descriptorWrites);
		}
	}
}

