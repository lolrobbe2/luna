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
		VkResult vulkanObjectFactory::createImageView(VkImageView* pImageView, VkImageViewCreateInfo* pImageViewCreateInfo = nullptr)
		{
			ref<vulkan::vulkanDevice> device = std::dynamic_pointer_cast<vulkan::vulkanDevice>(pDevice);
			return vkCreateImageView(device->getDeviceHandles().device, pImageViewCreateInfo, nullptr,pImageView);
		}
		VkResult vulkanObjectFactory::createImageView(VkImageView* pImageView,const VkImage& image,const VkFormat& format,const VkImageAspectFlags& imageAspectFlags)
		{
			VkImageViewCreateInfo imageViewCreateInfo = {};
			imageViewCreateInfo.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
			imageViewCreateInfo.pNext = nullptr;

			imageViewCreateInfo.viewType = VK_IMAGE_VIEW_TYPE_2D;
			imageViewCreateInfo.image = image;
			imageViewCreateInfo.format = format;
			imageViewCreateInfo.subresourceRange.baseMipLevel = 0;
			imageViewCreateInfo.subresourceRange.levelCount = 1;
			imageViewCreateInfo.subresourceRange.baseArrayLayer = 0;
			imageViewCreateInfo.subresourceRange.layerCount = 1;
			imageViewCreateInfo.subresourceRange.aspectMask = imageAspectFlags;
			return createImageView(pImageView, &imageViewCreateInfo);
		}
	}
}
