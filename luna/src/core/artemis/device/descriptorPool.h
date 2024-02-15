#pragma once
#include <core/artemis/device/descriptorSet.h>
namespace luna 
{
	namespace artemis 
	{
		class descriptorPool
		{
		public:
			descriptorPool() = default;
			/**
			 * @brief allocates a descripor set from this pool.
			 * @brief WARNING the descriptor set gets destroyed when the destructor is called!
			 */
			descriptorSet& allocateDescriptorSet();
			std::vector<descriptorSet>& allocateDescriptorSets(uint32_t amount);
		protected:
			friend class descriptorPoolBuilder;
			descriptorPool(const VkDevice* device, VkDescriptorPoolCreateInfo info, std::vector<VkWriteDescriptorSet>& descriptorWrites,VkDescriptorSetLayout layout);
		public:
			operator VkDescriptorPool() { return _descriptorPool; }
			operator VkDescriptorPool*() { return &_descriptorPool; }

			operator VkDescriptorSetLayout() { return layout; }
			operator VkDescriptorSetLayout*() { return &layout; }
		
		private:
			const VkDevice* p_device = nullptr;
			VkDescriptorPool _descriptorPool = VK_NULL_HANDLE;
			VkDescriptorSetLayout layout = VK_NULL_HANDLE;
			std::vector<VkWriteDescriptorSet> descriptorWrites;
		};
	}
}


