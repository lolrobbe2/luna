#pragma once
#include <core/platform/windows/windowsWindow.h>
#define DESCRIPTOR_TYPE_AMOUNT 16

#define SAMPLER_AMOUNT 0
#define COMBINED_IMAGE_SAMPLER_AMOUNT 1
#define SAMPLED_IMAGE_AMOUNT 2
#define STORAGE_IMAGE_AMOUNT 3
#define UNIFORM_TEXEL_BUFFER_AMOUNT 4
#define STORAGE_TEXEL_BUFFER_AMOUNT 5
#define UNIFORM_BUFFER_AMOUNT 6
#define STORAGE_BUFFER_AMOUNT 7
#define UNIFORM_BUFFER_DYNAMIC_AMOUNT 8
#define STORAGE_BUFFER_DYNAMIC_AMOUNT 9
#define INPUT_ATTACHMENT_AMOUNT 10
#define INLINE_UNIFORM_BLOCK_AMOUNT 11
#define ACCELERATION_STRUCTURE_KHR_AMOUNT 12
#define ACCELERATION_STRUCTURE_NV_AMOUNT 13
#define MUTABLE_VALVE_AMOUNT 14
#define INLINE_UNIFORM_BLOCK_EXT_AMOUNT 15
namespace luna 
{
	namespace artemis 
	{
		class shader; // forward declared so that it can be put in the device.h class
		class descriptorPoolBuilder
		{
		protected: 
			friend class device;
			descriptorPoolBuilder(const VkDevice* device, const ref<shader> shader);

		public:
			descriptorPoolBuilder& setSamplerAmount(uint32_t amount)				  { poolSizes[SAMPLER_AMOUNT].descriptorCount = amount; }
			descriptorPoolBuilder& setCombinedImageSamplerAmount(uint32_t amount)	  { poolSizes[COMBINED_IMAGE_SAMPLER_AMOUNT].descriptorCount = amount; }
			descriptorPoolBuilder& setSampledImageAmount(uint32_t amount)			  { poolSizes[SAMPLED_IMAGE_AMOUNT].descriptorCount = amount; }
			descriptorPoolBuilder& setStorageImageAmount(uint32_t amount)			  { poolSizes[STORAGE_IMAGE_AMOUNT].descriptorCount = amount; }
			descriptorPoolBuilder& setUniformTexelBuffer(uint32_t amount)			  { poolSizes[UNIFORM_TEXEL_BUFFER_AMOUNT].descriptorCount = amount; }
			descriptorPoolBuilder& setStorageTexelBuffer(uint32_t amount)			  { poolSizes[STORAGE_TEXEL_BUFFER_AMOUNT].descriptorCount = amount; }
			descriptorPoolBuilder& setUniformBufferAmount(uint32_t amount)		      { poolSizes[UNIFORM_BUFFER_AMOUNT].descriptorCount = amount; }
			descriptorPoolBuilder& setStorageBufferAmount(uint32_t amount)			  { poolSizes[STORAGE_BUFFER_AMOUNT].descriptorCount = amount; }
			descriptorPoolBuilder& setUniformBufferDynamicAmount(uint32_t amount)	  { poolSizes[UNIFORM_BUFFER_DYNAMIC_AMOUNT].descriptorCount = amount; }
			descriptorPoolBuilder& setStorageBufferDynamicAmount(uint32_t amount)	  { poolSizes[STORAGE_BUFFER_DYNAMIC_AMOUNT].descriptorCount = amount; }
			descriptorPoolBuilder& setInputAttachementAmount(uint32_t amount)		  { poolSizes[INPUT_ATTACHMENT_AMOUNT].descriptorCount = amount; }
			descriptorPoolBuilder& setInlineUniformBlockAmount(uint32_t amount)		  { poolSizes[INLINE_UNIFORM_BLOCK_AMOUNT].descriptorCount = amount; }
			descriptorPoolBuilder& setAccelerationStructureKhrAmount(uint32_t amount) { poolSizes[ACCELERATION_STRUCTURE_KHR_AMOUNT].descriptorCount = amount; }
			descriptorPoolBuilder& setAccelerationStructureNvAmount(uint32_t amount)  { poolSizes[ACCELERATION_STRUCTURE_NV_AMOUNT].descriptorCount = amount; }
			descriptorPoolBuilder& setMutableValveAmount(uint32_t amount)			  { poolSizes[MUTABLE_VALVE_AMOUNT].descriptorCount = amount; }
			descriptorPoolBuilder& setInlineUniformBlockExtAmount(uint32_t amount)    { poolSizes[INLINE_UNIFORM_BLOCK_EXT_AMOUNT].descriptorCount = amount; }
			descriptorPoolBuilder& setMaxSets(uint32_t maxAmount = 100)					  { info.maxSets = maxAmount; }

		private:
			void setDescriptorTypes();
			const VkDevice* p_device;
			VkDescriptorPoolCreateInfo info{ VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO };
			VkDescriptorPoolSize poolSizes[DESCRIPTOR_TYPE_AMOUNT];
		};
	}
}


