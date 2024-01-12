#include "descriptorPoolBuilder.h"
#include <core/artemis/rendering/shader.h>
namespace luna 
{
	namespace artemis 
	{
		descriptorPoolBuilder::descriptorPoolBuilder(const VkDevice* device, const ref<shader> shader)
		{
			p_device = device;
            setDescriptorTypes();
            info.poolSizeCount = DESCRIPTOR_TYPE_AMOUNT;
            info.pPoolSizes = poolSizes;
            info.maxSets = 100;
           
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
	}
}

