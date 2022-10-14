#include "vulkanAlllocator.h"
#include <core/vulkan/device/vulkanDevice.h>
namespace luna
{
	namespace utils
	{
		vulkanAllocator::vulkanAllocator(ref<renderer::device> device)
		{
			VmaAllocatorCreateInfo allocatorCreateInfo;
			ref<vulkan::vulkanDevice> vDevice = std::dynamic_pointer_cast<vulkan::vulkanDevice>(device);
			vulkan::vulkanDevice::deviceHandles handles = vDevice->getDeviceHandles();
			allocatorCreateInfo.instance = handles.instance;
			allocatorCreateInfo.physicalDevice = handles.physicalDevice;
			allocatorCreateInfo.device = handles.device;
			VkPhysicalDeviceMemoryProperties memoryProperties;
			vkGetPhysicalDeviceMemoryProperties(handles.physicalDevice, &memoryProperties);
			allocatorCreateInfo.pHeapSizeLimit = (VkDeviceSize*)&memoryProperties.memoryHeapCount;
			allocatorCreateInfo.vulkanApiVersion = handles.appInfo.apiVersion;
			LN_CORE_INFO("vma allocator creation result = ",vmaCreateAllocator(&allocatorCreateInfo, &sAllocator));
		}
		vulkanAllocator::~vulkanAllocator()
		{
			if (sAllocator == VK_NULL_HANDLE) return;
			vmaDestroyAllocator(sAllocator);
		}
	}
}

