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
		VkResult vulkanDescriptorPool::createDescriptorSet(VkDescriptorSet* descriptorSet)
		{
			//vkAllocateDescriptorSets(device->getDeviceHandles().device,
			return VkResult();
		}
	}
}
