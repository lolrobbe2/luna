#pragma once
#include <core/artemis/device/descriptorSet.h>
namespace luna 
{
	namespace artemis 
	{
		class descriptorPool
		{
		protected:
			friend class descriptorPoolBuilder;
			descriptorPool(const VkDevice* device, VkDescriptorPoolCreateInfo info, std::vector<VkWriteDescriptorSet>& descriptorWrites);
			/**
			* @brief allocates a descripor set from this pool.
			* @brief WARNING the descriptor set gets destroyed when the destructor is called! 
			*/
			descriptorSet& allocateDescriptorSet();
			std::vector<descriptorSet>& allocateDescriptorSets();
		public:
			operator VkDescriptorPool() { return _descriptorPool; }
			operator VkDescriptorPool*() { return &_descriptorPool; }

			operator VkDescriptorSetLayout() { return layout; }
			operator VkDescriptorSetLayout*() { return &layout; }
		
		private:
			const VkDevice* p_device;
			VkDescriptorPool _descriptorPool;
			VkDescriptorSetLayout layout;
			std::vector<VkWriteDescriptorSet> descriptorWrites;
		};
	}
}


