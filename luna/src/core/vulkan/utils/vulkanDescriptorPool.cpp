#include "vulkanDescriptorPool.h"
namespace luna
{
	namespace utils 
	{
		vulkanDescriptorPool::vulkanDescriptorPool(const ref<vulkan::vulkanDevice>& pDevice, VkDescriptorPoolSize* pPoolSizes,const uint32_t& poolSizeCount, const std::vector<renderer::shaderResource>& shaderLayout, VkDescriptorSetLayoutCreateFlags flags)
		{
			LN_PROFILE_FUNCTION();
			VkDescriptorPoolCreateInfo createInfo{ VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO };
			createInfo.maxSets = 1000;
			createInfo.pPoolSizes = pPoolSizes;
			createInfo.poolSizeCount = poolSizeCount;
			createInfo.pNext = nullptr;
			device = pDevice;
			vkCreateDescriptorPool(device->getDeviceHandles().device,&createInfo,nullptr,&descriptorPool);		
			layout = createDescriptorLayouts(shaderLayout, flags);
			createDescriptorWrites(shaderLayout);
		}
		VkResult vulkanDescriptorPool::createDescriptorSets(std::vector<ref<vulkanDescriptorSet>>& descriptorSets)
		{
			//remeber descriptorSets handles have not been initialized;
			VkDescriptorSetAllocateInfo allocateInfo {VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO};
			allocateInfo.descriptorPool = descriptorPool;
			allocateInfo.descriptorSetCount = descriptorSets.size();
			std::vector<VkDescriptorSetLayout> layouts(descriptorSets.size(), layout);
			allocateInfo.pSetLayouts = layouts.data();
			std::vector<VkDescriptorSet> descriptors;
			descriptors.resize(descriptorSets.size());
			vkAllocateDescriptorSets(device->getDeviceHandles().device, &allocateInfo,descriptors.data());
			for (size_t i = 0; i < descriptors.size(); i++)
			{
				descriptorSets[i] = ref<vulkanDescriptorSet>(new vulkanDescriptorSet(device, descriptors[i], descriptorPool, descriptorWrites));
			}
			return VkResult();
		}
		VkDescriptorSetLayout vulkanDescriptorPool::createDescriptorLayouts(const std::vector<renderer::shaderResource>& shaderLayout, VkDescriptorSetLayoutCreateFlags flags)
		{
			LN_PROFILE_FUNCTION();
			VkDescriptorSetLayout setLayout;
			VkDescriptorSetLayoutCreateInfo setLayoutCreateInfo{ VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO };
			std::vector<VkDescriptorSetLayoutBinding> resourceLayoutBindings;
			for (size_t i = shaderLayout.size() - 1; i > 0; i--)
			{
				VkDescriptorSetLayoutBinding resourceLayoutBinding;
				switch (shaderLayout[i].resourceClass)
				{
				case renderer::uniformBuffers:
					LN_CORE_ERROR("uniform buffers not implemented!");
					break;
				case renderer::storageBuffers:
					LN_CORE_ERROR("storage buffers not implemented!");
					break;
				case renderer::storageImages:
					resourceLayoutBinding.binding = shaderLayout[i].binding;
					resourceLayoutBinding.descriptorType = VK_DESCRIPTOR_TYPE_SAMPLED_IMAGE;
					resourceLayoutBinding.descriptorCount = shaderLayout[i].amount;
					resourceLayoutBinding.pImmutableSamplers = nullptr;
					resourceLayoutBinding.stageFlags = VkShaderStageFlagBits::VK_SHADER_STAGE_FRAGMENT_BIT;
					resourceLayoutBindings.push_back(resourceLayoutBinding);
					break;
				case renderer::sampledImages:
					//TODO create samplers perhaps?
					LN_CORE_ERROR("sampled images not implemented!");
					break;
				default:

					switch (shaderLayout[i].type)
					{
					case renderer::Sampler:
						resourceLayoutBinding.binding = shaderLayout[i].binding;
						resourceLayoutBinding.descriptorType = VK_DESCRIPTOR_TYPE_SAMPLER;
						resourceLayoutBinding.descriptorCount = 1; //TODO make dynamic!!!
						resourceLayoutBinding.pImmutableSamplers = nullptr;
						resourceLayoutBinding.stageFlags = VkShaderStageFlagBits::VK_SHADER_STAGE_FRAGMENT_BIT;
						resourceLayoutBindings.push_back(resourceLayoutBinding);
						LN_CORE_ERROR("found that sampler");
					default:
						break;
					}	
				}
			}
			setLayoutCreateInfo.bindingCount = resourceLayoutBindings.size();
			setLayoutCreateInfo.pBindings = resourceLayoutBindings.data();
			setLayoutCreateInfo.flags = 0;	
			setLayoutCreateInfo.pNext = nullptr;
			vkCreateDescriptorSetLayout(device->getDeviceHandles().device, &setLayoutCreateInfo, nullptr, &setLayout);
			return setLayout;
		}

		void vulkanDescriptorPool::createDescriptorWrites(const std::vector<renderer::shaderResource>& shaderLayout)
		{
			descriptorWrites.resize(1);
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
					if (descriptorWrites.size() <= resource.binding) descriptorWrites.resize(resource.binding + 1);
					descriptorWrites[resource.binding].sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
					descriptorWrites[resource.binding].descriptorType = VK_DESCRIPTOR_TYPE_SAMPLED_IMAGE;
					descriptorWrites[resource.binding].descriptorCount = resource.amount;
					descriptorWrites[resource.binding].dstBinding = resource.binding;
					break;
				case renderer::sampledImages:
					//TODO create samplers perhaps?
					LN_CORE_ERROR("sampled images not implemented!");
					break;
				}
				switch (resource.type)
				{
				case renderer::Sampler:
					if (descriptorWrites.size() <= resource.binding) descriptorWrites.resize(resource.binding + 1);
					descriptorWrites[resource.binding].sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
					descriptorWrites[resource.binding].descriptorType = VK_DESCRIPTOR_TYPE_SAMPLER;
					descriptorWrites[resource.binding].descriptorCount = 1;
					descriptorWrites[resource.binding].dstBinding = resource.binding;
					break;
				default:
					break;
				}
			}
			
		}

		/*------------------------------------------(descriptor set implementation)------------------------------------------*/

		vulkanDescriptorSet::vulkanDescriptorSet(const ref<vulkan::vulkanDevice>& device, const VkDescriptorSet& descriptorSet, const VkDescriptorPool& descriptorPool, const std::vector<VkWriteDescriptorSet>& descriptorWrites)
		{
			this->descriptorSet = descriptorSet;
			this->descriptorPool = descriptorPool;
			this->device = device;
			this->descriptorWrites = descriptorWrites;
			for (auto& write : this->descriptorWrites) write.dstSet = descriptorSet;
		}
		VkResult vulkanDescriptorSet::write(const uint32_t& descriptorIndex, void* pDescriptorInfo)
		{

			switch (descriptorWrites[descriptorIndex].descriptorType)
			{
			case VK_DESCRIPTOR_TYPE_SAMPLER:
				descriptorWrites[descriptorIndex].pImageInfo = (VkDescriptorImageInfo*)pDescriptorInfo;
				break;
			case VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER:
				descriptorWrites[descriptorIndex].pImageInfo = (VkDescriptorImageInfo*)pDescriptorInfo;
				break;
			case VK_DESCRIPTOR_TYPE_SAMPLED_IMAGE:
				descriptorWrites[descriptorIndex].pImageInfo = (VkDescriptorImageInfo*)pDescriptorInfo;
				break;
			case VK_DESCRIPTOR_TYPE_STORAGE_IMAGE:
				descriptorWrites[descriptorIndex].pImageInfo = (VkDescriptorImageInfo*)pDescriptorInfo;
				break;
			case VK_DESCRIPTOR_TYPE_UNIFORM_TEXEL_BUFFER:
				descriptorWrites[descriptorIndex].pBufferInfo = (VkDescriptorBufferInfo*)pDescriptorInfo;
				break;
			case VK_DESCRIPTOR_TYPE_STORAGE_TEXEL_BUFFER:
				descriptorWrites[descriptorIndex].pBufferInfo = (VkDescriptorBufferInfo*)pDescriptorInfo;
				break;
			case VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER:
				descriptorWrites[descriptorIndex].pBufferInfo = (VkDescriptorBufferInfo*)pDescriptorInfo;
				break;
			case VK_DESCRIPTOR_TYPE_STORAGE_BUFFER:
				descriptorWrites[descriptorIndex].pBufferInfo = (VkDescriptorBufferInfo*)pDescriptorInfo;
				break;
			case VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER_DYNAMIC:
				descriptorWrites[descriptorIndex].pBufferInfo = (VkDescriptorBufferInfo*)pDescriptorInfo;
				break;
			case VK_DESCRIPTOR_TYPE_STORAGE_BUFFER_DYNAMIC:
				descriptorWrites[descriptorIndex].pBufferInfo = (VkDescriptorBufferInfo*)pDescriptorInfo;
				break;
			default:
				break;
			}
			//imageview not valid?
			vkDeviceWaitIdle(device->getDeviceHandles().device);
			vkUpdateDescriptorSets(device->getDeviceHandles().device, 1, &descriptorWrites[descriptorIndex], 0, nullptr);
			return VkResult();
		}
	}
}
