#include "descriptorPoolBuilder.h"
#include <core/artemis/rendering/shader.h>
#include <core/debug/debugMacros.h>
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
			return descriptorPool(p_device,info,createDescriptorWrites(),createLayout(layoutFlags));
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
		std::vector<VkWriteDescriptorSet> descriptorPoolBuilder::createDescriptorWrites()
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
					if (descriptorWrites.size() <= resource.binding) descriptorWrites.resize(resource.binding + 1);
					descriptorWrites[resource.binding].sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
					descriptorWrites[resource.binding].descriptorType = VK_DESCRIPTOR_TYPE_STORAGE_BUFFER;
					descriptorWrites[resource.binding].descriptorCount = resource.amount;
					descriptorWrites[resource.binding].dstBinding = resource.binding;
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
		const VkDescriptorSetLayout descriptorPoolBuilder::createLayout(VkDescriptorSetLayoutCreateFlags flags)
		{

			LN_PROFILE_FUNCTION();
			VkDescriptorSetLayout setLayout;
			VkDescriptorSetLayoutCreateInfo setLayoutCreateInfo{ VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO };
			std::vector<VkDescriptorSetLayoutBinding> resourceLayoutBindings;
			std::vector<shaderResource> shaderLayout = p_shader->layout();
			//question: why did i ever reverse this for loop.
			for (size_t i = 0; i < shaderLayout.size(); i++)
			{
				VkDescriptorSetLayoutBinding resourceLayoutBinding;
				switch (shaderLayout[i].resourceClass)
				{
				case uniformBuffers:
					LN_CORE_ERROR("uniform buffers not implemented!");
					break;
				case storageBuffers:
					resourceLayoutBinding.binding = shaderLayout[i].binding;
					resourceLayoutBinding.descriptorType = VK_DESCRIPTOR_TYPE_STORAGE_BUFFER;
					resourceLayoutBinding.descriptorCount = shaderLayout[i].amount;
					resourceLayoutBinding.pImmutableSamplers = nullptr;
					resourceLayoutBinding.stageFlags = VkShaderStageFlagBits::VK_SHADER_STAGE_COMPUTE_BIT;
					resourceLayoutBindings.push_back(resourceLayoutBinding);
					break;
				case storageImages:
					resourceLayoutBinding.binding = shaderLayout[i].binding;
					resourceLayoutBinding.descriptorType = VK_DESCRIPTOR_TYPE_SAMPLED_IMAGE;
					resourceLayoutBinding.descriptorCount = shaderLayout[i].amount;
					resourceLayoutBinding.pImmutableSamplers = nullptr;
					resourceLayoutBinding.stageFlags = VkShaderStageFlagBits::VK_SHADER_STAGE_FRAGMENT_BIT;
					resourceLayoutBindings.push_back(resourceLayoutBinding);
					break;
				case sampledImages:
					break;
				default:

					switch (shaderLayout[i].type)
					{
					case Sampler:
						resourceLayoutBinding.binding = shaderLayout[i].binding;
						resourceLayoutBinding.descriptorType = VK_DESCRIPTOR_TYPE_SAMPLER;
						resourceLayoutBinding.descriptorCount = 1; //TODO make dynamic!!!
						resourceLayoutBinding.pImmutableSamplers = nullptr;
						resourceLayoutBinding.stageFlags = VkShaderStageFlagBits::VK_SHADER_STAGE_FRAGMENT_BIT;
						resourceLayoutBindings.push_back(resourceLayoutBinding);
					default:
						break;
					}
				}
			}
			setLayoutCreateInfo.bindingCount = resourceLayoutBindings.size();
			setLayoutCreateInfo.pBindings = resourceLayoutBindings.data();
			setLayoutCreateInfo.flags = flags;
			setLayoutCreateInfo.pNext = nullptr;
			VkResult createRes = vkCreateDescriptorSetLayout(*p_device, &setLayoutCreateInfo, nullptr, &setLayout);
			
			LN_ERR_FAIL_COND_V_MSG(createRes != VK_SUCCESS, VK_NULL_HANDLE, "[Artemis] an error occured whilst creating the descriptorSet layout, VkResult: " + VK_RESULT(createRes));

			return setLayout;
		}
	}
}

