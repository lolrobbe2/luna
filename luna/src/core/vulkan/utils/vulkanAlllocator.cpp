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
			pDevice = device;
		}
		vulkanAllocator::~vulkanAllocator()
		{
			if (sAllocator == VK_NULL_HANDLE) return;
			vmaDestroyAllocator(sAllocator);
		}

		VkResult vulkanAllocator::createImage(VkImage* pImage, VkImageUsageFlags usageFlags,VmaMemoryUsage memoryUsage, VkExtent3D extent, VkFormat format)
		{
		
			ref<vulkan::vulkanDevice> device = std::dynamic_pointer_cast<vulkan::vulkanDevice>(pDevice);
			VkImageCreateInfo imageCreateInfo = { VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO };
			imageCreateInfo.pNext = nullptr;

			imageCreateInfo.imageType = VK_IMAGE_TYPE_2D;

			imageCreateInfo.format = format;
			imageCreateInfo.extent = extent;

			imageCreateInfo.mipLevels = 1;
			imageCreateInfo.arrayLayers = 1;
			imageCreateInfo.samples = VK_SAMPLE_COUNT_1_BIT;
			imageCreateInfo.tiling = VK_IMAGE_TILING_OPTIMAL;
			imageCreateInfo.usage = usageFlags;

			VmaAllocation allocation;

			VmaAllocationCreateInfo allocationCreateInfo;
			allocationCreateInfo.pool = NULL;
			allocationCreateInfo.memoryTypeBits = device->getDeviceHandles().physicalDevice.memory_properties.memoryTypes->propertyFlags;
			allocationCreateInfo.usage = memoryUsage;
			VmaAllocationInfo allocationInfo;
			return vmaCreateImage(sAllocator, &imageCreateInfo, &allocationCreateInfo, pImage, &allocation, &allocationInfo);

		}
	}
}

