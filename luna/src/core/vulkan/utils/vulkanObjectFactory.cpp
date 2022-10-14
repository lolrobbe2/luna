#include "vulkanObjectFactory.h"
#include <core/vulkan/device/vulkanDevice.h>

namespace luna
{
	namespace utils
	{
		VkResult vulkanObjectFactory::init(ref<renderer::device> device)
		{
			pDevice = device;
			return VK_SUCCESS;
		}

		VkResult vulkanObjectFactory::createImage(VkImage* pImage,VkImageCreateInfo* pCreateInfo = nullptr)
		{
			ref<vulkan::vulkanDevice> device = std::dynamic_pointer_cast<vulkan::vulkanDevice>(pDevice);
			return vkCreateImage(device->getDeviceHandles().device, pCreateInfo,nullptr ,pImage);
		}

		VkResult vulkanObjectFactory::createImage(VkImage* pImage, VkImageUsageFlags usageFlags, VkExtent3D extent, VkFormat format)
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
			return vkCreateImage(device->getDeviceHandles().device, &imageCreateInfo, nullptr, pImage);
		}
	}
}
