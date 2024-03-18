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
			info.flags |= VK_DESCRIPTOR_POOL_CREATE_FREE_DESCRIPTOR_SET_BIT;
			VkResult createResult = vkCreateDescriptorPool(*device, &info, nullptr, &tempDescriptorPool);
			LN_ERR_FAIL_COND_MSG(createResult != VK_SUCCESS, "[Artemis] unable to create descriptor set, VkResult:  " + VK_RESULT(createResult));
			
			p_device = device;
			_descriptorPool = tempDescriptorPool;
			this->descriptorWrites = descriptorWrites;
			this->layout = layout;
		}
		descriptorSet& descriptorPool::allocateDescriptorSet()
		{
			LN_ERR_FAIL_COND_V_MSG(_descriptorPool == VK_NULL_HANDLE, descriptorSet(), "[Artemis] descriptorPool has not been initialized!");
			VkDescriptorSetAllocateInfo allocateInfo{ VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO };
			allocateInfo.descriptorPool = _descriptorPool;
			allocateInfo.descriptorSetCount = 1;
			allocateInfo.pSetLayouts = &layout;
			VkDescriptorSet set;
			vkAllocateDescriptorSets(*p_device, &allocateInfo, &set);
			return *new descriptorSet(p_device,set,&_descriptorPool,descriptorWrites);
		}
		std::vector<descriptorSet>& descriptorPool::allocateDescriptorSets(uint32_t amount)
		{
			VkDescriptorSetAllocateInfo allocateInfo{ VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO };
			allocateInfo.descriptorPool = _descriptorPool;
			allocateInfo.descriptorSetCount = amount;
			std::vector<VkDescriptorSetLayout> layouts(amount, layout);
			allocateInfo.pSetLayouts = layouts.data();
			std::vector<VkDescriptorSet> nativeSets(amount);
			VkResult allocRes = vkAllocateDescriptorSets(*p_device, &allocateInfo, nativeSets.data());
			
			LN_ERR_FAIL_COND_V_MSG(allocRes != VK_SUCCESS, std::vector<descriptorSet>(), "[Artemis] an error occured during descriptorSets allocation, VkResult: " + VK_RESULT(allocRes));

			std::vector<descriptorSet>* sets = new std::vector<descriptorSet>();
			sets->resize(amount); //reserve upfront for performance reasons

			for (size_t i = 0; i < sets->size();i++)
			{
				(*sets)[i] = *new descriptorSet(p_device, nativeSets[i],&_descriptorPool,descriptorWrites);
			}
			return *sets;
		}
	}
}

