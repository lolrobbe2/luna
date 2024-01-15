#include "descriptorPool.h"
#include <core/debug/debugMacros.h>
namespace luna 
{
	namespace artemis 
	{
		descriptorPool::descriptorPool(const VkDevice* device,VkDescriptorPoolCreateInfo info, std::vector<VkWriteDescriptorSet>& descriptorWrites,VkDescriptorSetLayout layout)
		{
			VkDescriptorPool tempDescriptorPool; //temp used to make sure that when an error occured _desscriptorPool stays VK_NULL_HANDLE
			
			//VkDescriptorPoolSize::descriptorCount = 0 is not valid so only add valid ones!
			std::vector<VkDescriptorPoolSize> poolSizes;
			for (size_t i = 0; i < info.poolSizeCount; i++)
			{
				if (info.pPoolSizes[i].descriptorCount > 0) poolSizes.push_back(info.pPoolSizes[i]);
			}

			info.pPoolSizes = poolSizes.data();
			info.poolSizeCount = poolSizes.size();

			VkResult createResult = vkCreateDescriptorPool(*device, &info, nullptr, &tempDescriptorPool);
			LN_ERR_FAIL_COND_MSG(createResult != VK_SUCCESS, "[Artemis] unable to create descriptor set, VkResult:  " + std::to_string(createResult));
			
			p_device = device;
			_descriptorPool = tempDescriptorPool;
			this->descriptorWrites = descriptorWrites;
			this->layout = layout;
		}
		descriptorSet& descriptorPool::allocateDescriptorSet()
		{
			VkDescriptorSetAllocateInfo allocateInfo{ VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO };
			allocateInfo.descriptorPool = _descriptorPool;
			allocateInfo.descriptorSetCount = 1;
			allocateInfo.pSetLayouts = &layout;
			VkDescriptorSet set;
			vkAllocateDescriptorSets(*p_device, &allocateInfo, &set);
			return descriptorSet(set,&_descriptorPool,descriptorWrites);
		}
		std::vector<descriptorSet>& descriptorPool::allocateDescriptorSets()
		{
			return std::vector<descriptorSet>();
		}
	}
}

