#pragma once
#include <core/platform/windows/windowsWindow.h>
namespace luna 
{
	namespace artemis 
	{
		class descriptorPool
		{
		protected:
			descriptorPool(const VkDevice* device, VkDescriptorPoolCreateInfo info, std::vector<VkWriteDescriptorSet>& descriptorWrites);
		public:
			operator VkDescriptorPool() { return _descriptorPool; }
			operator VkDescriptorPool*() { return &_descriptorPool; }

			operator VkDescriptorSetLayout() { return layout; }
			operator VkDescriptorSetLayout*() { return &layout; }
		
		private:
			VkDescriptorPool _descriptorPool;
			VkDescriptorSetLayout layout;
			std::vector<VkWriteDescriptorSet> descriptorWrites;
		};
	}
}


