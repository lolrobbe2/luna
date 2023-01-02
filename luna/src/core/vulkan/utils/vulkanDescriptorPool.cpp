#include "vulkanDescriptorPool.h"
namespace luna
{
	namespace utils 
	{
		vulkanDescriptorPool::vulkanDescriptorPool(VkDescriptorPoolSize* pPoolSizes,const uint32_t& poolSizeCount )
		{
			VkDescriptorPoolCreateInfo createInfo{ VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO };
			createInfo.maxSets = 1000;
			createInfo.pPoolSizes = pPoolSizes;
			createInfo.poolSizeCount = poolSizeCount;
			createInfo.pNext = nullptr;
			vkCreateDescriptorPool(device->getDeviceHandles().device,&createInfo,nullptr,&descriptorPool);		
		}
		VkResult vulkanDescriptorPool::createDescriptorSets(VkDescriptorSet* pDescriptorSets, uint64_t amount)
		{
			VkDescriptorSetAllocateInfo allocateInfo {VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO};
			allocateInfo.descriptorPool = descriptorPool;
			allocateInfo.descriptorSetCount = amount;
			allocateInfo.pSetLayouts = nullptr;
			
			vkAllocateDescriptorSets(device->getDeviceHandles().device, &allocateInfo, pDescriptorSets);
			return VkResult();
		}
		VkDescriptorSetLayout* vulkanDescriptorPool::createDescriptorLayouts(const std::vector<renderer::shaderResource>& shaderLayout, VkDescriptorSetLayoutCreateFlags flags)
		{
			VkDescriptorSetLayout setLayout;
			VkDescriptorSetLayoutCreateInfo setLayoutCreateInfo{ VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO };
			std::vector<VkDescriptorSetLayoutBinding> resourceLayoutBindings;
			for (renderer::shaderResource resource : shaderLayout)
			{
				VkDescriptorSetLayoutBinding resourceLayoutBinding;
				switch (resource.resourceClass)
				{
				case renderer::uniformBuffers:
					LN_CORE_ERROR("uniform buffers not implemented!");
					break;
				case renderer::storageBuffers:
					LN_CORE_ERROR("storage buffers not implemented!");
					break;
				case renderer::storageImages:
					resourceLayoutBinding.binding = resource.binding;
					resourceLayoutBinding.descriptorType = VK_DESCRIPTOR_TYPE_SAMPLED_IMAGE;
					resourceLayoutBinding.descriptorCount = resource.amount;
					resourceLayoutBinding.pImmutableSamplers = nullptr;
					resourceLayoutBindings.push_back(resourceLayoutBinding);
					break;
				case renderer::sampledImages:
					//TODO create samplers perhaps?
					LN_CORE_ERROR("sampled images not implemented!");
					break;

				default:
					LN_CORE_INFO("not supported inside of descriptorsets: {0}", resource.resourceClass);
					break;
				}
			}
			setLayoutCreateInfo.bindingCount = resourceLayoutBindings.size();
			setLayoutCreateInfo.pBindings = resourceLayoutBindings.data();
			setLayoutCreateInfo.flags = 0;	
			setLayoutCreateInfo.pNext = nullptr;
			vkCreateDescriptorSetLayout(device->getDeviceHandles().device, &setLayoutCreateInfo, nullptr, &setLayout);
			return nullptr;
		}

		/*------------------------------------------(descriptor set implementation)------------------------------------------*/

		vulkanDescriptorSet::vulkanDescriptorSet(const ref<vulkan::vulkanDevice>& device, const VkDescriptorSet& descriptoSet, const VkDescriptorPool& descriptorPool, const VkWriteDescriptorSet& descriptorWrites)
		{
			descriptorSet = descriptorSet;
			this->descriptorPool = descriptorPool;
			this->device = device;
		}
		VkResult vulkanDescriptorSet::write()
		{
			//descriptorWrite.
			vkUpdateDescriptorSets(device->getDeviceHandles().device, 1, &descriptorWrite,0,nullptr);
			return VkResult();
		}
	}
}
