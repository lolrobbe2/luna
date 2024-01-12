#include "descriptorPoolBuilder.h"
#include <core/artemis/rendering/shader.h>
namespace luna
{
	namespace artemis 
	{
		descriptorPoolBuilder::descriptorPoolBuilder(const VkDevice* device, const ref<shader> shader)
		{
			p_device = device;
			p_shader = shader;
            setDescriptorTypes();
            info.poolSizeCount = DESCRIPTOR_TYPE_AMOUNT;
            info.pPoolSizes = poolSizes;
            info.maxSets = 100;
			
           
		}
		descriptorPool descriptorPoolBuilder::build()
		{
			return descriptorPool(p_device,info,createDescriptorWrites());
		}
		void descriptorPoolBuilder::setDescriptorTypes()
		{
            poolSizes[SAMPLER_AMOUNT] = { VK_DESCRIPTOR_TYPE_SAMPLER, 0 };
            poolSizes[COMBINED_IMAGE_SAMPLER_AMOUNT] = { VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, 0 };
            poolSizes[SAMPLED_IMAGE_AMOUNT] = { VK_DESCRIPTOR_TYPE_SAMPLED_IMAGE, 0 };
            poolSizes[STORAGE_IMAGE_AMOUNT] = { VK_DESCRIPTOR_TYPE_STORAGE_IMAGE, 0 };
            poolSizes[UNIFORM_TEXEL_BUFFER_AMOUNT] = { VK_DESCRIPTOR_TYPE_UNIFORM_TEXEL_BUFFER, 0 };
            poolSizes[STORAGE_TEXEL_BUFFER_AMOUNT] = { VK_DESCRIPTOR_TYPE_STORAGE_TEXEL_BUFFER, 0 };
            poolSizes[UNIFORM_BUFFER_AMOUNT] = { VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, 0 };
            poolSizes[STORAGE_BUFFER_AMOUNT] = { VK_DESCRIPTOR_TYPE_STORAGE_BUFFER, 0 };
            poolSizes[UNIFORM_BUFFER_DYNAMIC_AMOUNT] = { VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER_DYNAMIC, 0 };
            poolSizes[STORAGE_BUFFER_DYNAMIC_AMOUNT] = { VK_DESCRIPTOR_TYPE_STORAGE_BUFFER_DYNAMIC, 0 };
            poolSizes[INPUT_ATTACHMENT_AMOUNT] = { VK_DESCRIPTOR_TYPE_INPUT_ATTACHMENT, 0 };
            poolSizes[INLINE_UNIFORM_BLOCK_AMOUNT] = { VK_DESCRIPTOR_TYPE_INLINE_UNIFORM_BLOCK_EXT, 0 };
            poolSizes[ACCELERATION_STRUCTURE_KHR_AMOUNT] = { VK_DESCRIPTOR_TYPE_ACCELERATION_STRUCTURE_KHR, 0 };
            poolSizes[ACCELERATION_STRUCTURE_NV_AMOUNT] = { VK_DESCRIPTOR_TYPE_ACCELERATION_STRUCTURE_NV, 0 };
            poolSizes[MUTABLE_VALVE_AMOUNT] = { VK_DESCRIPTOR_TYPE_MUTABLE_VALVE, 0 };
            poolSizes[INLINE_UNIFORM_BLOCK_EXT_AMOUNT] = { VK_DESCRIPTOR_TYPE_INLINE_UNIFORM_BLOCK_EXT, 0 };
		}
		std::vector<VkWriteDescriptorSet>& descriptorPoolBuilder::createDescriptorWrites()
		{
			LN_PROFILE_FUNCTION();
			std::vector<VkWriteDescriptorSet> descriptorWrites;
			descriptorWrites.resize(1);
			for (shaderResource resource : p_shader->layout())
			{
				VkDescriptorSetLayoutBinding resourceLayoutBinding;
				switch (resource.resourceClass)
				{
				case uniformBuffers:
					LN_CORE_ERROR("uniform buffers not implemented!");
					break;
				case storageBuffers:
					LN_CORE_ERROR("storage buffers not implemented!");
					break;
				case storageImages:
					if (descriptorWrites.size() <= resource.binding) descriptorWrites.resize(resource.binding + 1);
					descriptorWrites[resource.binding].sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
					descriptorWrites[resource.binding].descriptorType = VK_DESCRIPTOR_TYPE_SAMPLED_IMAGE;
					descriptorWrites[resource.binding].descriptorCount = resource.amount;
					descriptorWrites[resource.binding].dstBinding = resource.binding;
					break;
				case sampledImages:
					//TODO create samplers perhaps?
					LN_CORE_ERROR("sampled images not implemented!");
					break;
				}
				switch (resource.type)
				{
				case Sampler:
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
			return descriptorWrites;
		}
	}
}

