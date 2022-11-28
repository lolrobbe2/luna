#include "vulkanAllocator.h"
#include <core/vulkan/device/vulkanDevice.h>
namespace luna
{
	namespace utils
	{
		void vulkanAllocator::init(ref<renderer::device> device)
		{
			VmaAllocatorCreateInfo allocatorCreateInfo;
			ref<vulkan::vulkanDevice> vDevice = std::dynamic_pointer_cast<vulkan::vulkanDevice>(device);
			vulkan::vulkanDevice::deviceHandles handles = vDevice->getDeviceHandles();
			VmaVulkanFunctions vulkanFunctions = {};
			vulkanFunctions.vkGetInstanceProcAddr = &vkGetInstanceProcAddr;
			vulkanFunctions.vkGetDeviceProcAddr = &vkGetDeviceProcAddr;

			allocatorCreateInfo.instance = handles.instance;
			allocatorCreateInfo.physicalDevice = handles.physicalDevice;
			allocatorCreateInfo.device = handles.device;

			allocatorCreateInfo.pAllocationCallbacks = NULL;
			allocatorCreateInfo.pDeviceMemoryCallbacks = NULL;
			allocatorCreateInfo.pVulkanFunctions = NULL;

			allocatorCreateInfo.pTypeExternalMemoryHandleTypes = NULL;
			allocatorCreateInfo.pHeapSizeLimit = NULL;
			allocatorCreateInfo.preferredLargeHeapBlockSize = 0;

			VkPhysicalDeviceMemoryProperties memoryProperties;
			vkGetPhysicalDeviceMemoryProperties(handles.physicalDevice, &memoryProperties);
			allocatorCreateInfo.vulkanApiVersion = handles.appInfo.apiVersion;
			LN_CORE_INFO("vma allocator creation result = {0}", vmaCreateAllocator(&allocatorCreateInfo, &sAllocator));
			pDevice = device;
		}
		void vulkanAllocator::shutdown()
		{
			if (sAllocator == VK_NULL_HANDLE) return;
			vmaDestroyAllocator(sAllocator);
		}

		VmaAllocationInfo vulkanAllocator::getAllocationInfo(const uint64_t& handle)
		{
			return allocations.getValue(handle, vmaAllocation()).second.allocationInfo;
		}

		VkResult vulkanAllocator::createImage(VkImage* pImage, const VkImageUsageFlags& usageFlags, const VmaMemoryUsage& memoryUsage, const VkExtent3D& extent, const VkFormat& format)
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
			imageCreateInfo.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;

			VmaAllocation allocation;
			uint32_t memoryTypeIndex = 2;
			VmaAllocationCreateInfo allocationCreateInfo = {};
			allocationCreateInfo.usage = VMA_MEMORY_USAGE_AUTO;

			VmaAllocationInfo allocationInfo;
			VkResult createResult = vmaCreateImage(sAllocator, &imageCreateInfo, &allocationCreateInfo, pImage, &allocation, &allocationInfo);
			uint64_t handle = (uint64_t)*pImage;
			vmaAllocation vmaAlloc = { allocation,allocationInfo };
			allocations.putValue(&handle, vmaAlloc);
			return createResult;
		}
		VkResult vulkanAllocator::destroyImage(const VkImage& image)
		{
			auto result = allocations.getValue((uint64_t)image,vmaAllocation());
			if (result.first == storageOpSucces) vmaDestroyImage(sAllocator, image, result.second.allocation);
			allocations.eraseValue((uint64_t)image);
			return VK_SUCCESS; //TODO return appropriate VkResult based on object storage rsult.
		}

		VkResult vulkanAllocator::createImageView(VkImageView* pImageView, const VkImage& image, const VkFormat& format, const VkImageAspectFlags& imageAspectFlags)
		{
			ref<vulkan::vulkanDevice> device = std::dynamic_pointer_cast<vulkan::vulkanDevice>(pDevice);

			VkImageViewCreateInfo imageViewCreateInfo = { VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO };
			imageViewCreateInfo.pNext = nullptr;

			imageViewCreateInfo.viewType = VK_IMAGE_VIEW_TYPE_2D;
			imageViewCreateInfo.image = image;
			imageViewCreateInfo.format = format;
			imageViewCreateInfo.subresourceRange.baseMipLevel = 0;
			imageViewCreateInfo.subresourceRange.levelCount = 1;
			imageViewCreateInfo.subresourceRange.baseArrayLayer = 0;
			imageViewCreateInfo.subresourceRange.layerCount = 1;
			imageViewCreateInfo.subresourceRange.aspectMask = imageAspectFlags;

			return vkCreateImageView(device->getDeviceHandles().device, &imageViewCreateInfo, nullptr, pImageView);
		}
		VkResult vulkanAllocator::destroyImageView(const VkImageView& imageView)
		{
			ref<vulkan::vulkanDevice> device = std::dynamic_pointer_cast<vulkan::vulkanDevice>(pDevice);
			vkDestroyImageView(device->getDeviceHandles().device, imageView, nullptr);
			return VK_SUCCESS;
		}
		VkResult vulkanAllocator::createBuffer(VkBuffer* pBuffer, size_t allocSize, VkBufferUsageFlags usage, VmaMemoryUsage memoryUsage,VmaAllocationCreateFlags allocFlags)
		{
			VkBufferCreateInfo bufferInfo = {};
			bufferInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
			bufferInfo.pNext = nullptr;
			bufferInfo.size = allocSize;

			bufferInfo.usage = usage;


			//let the VMA library know that this data should be writeable by CPU, but also readable by GPU
			VmaAllocationCreateInfo vmaAllocInfo = {};
			vmaAllocInfo.usage = memoryUsage;
			vmaAllocInfo.flags = allocFlags;
			//VMA_ALLOCATION_CREATE_MAPPED_BIT;
			VkBuffer newBuffer;
			VmaAllocationInfo allocationInfo;
			VmaAllocation allocation;
			//allocate the buffer
			VkResult bufferCreateResult = vmaCreateBuffer(sAllocator, &bufferInfo, &vmaAllocInfo,pBuffer,&allocation,&allocationInfo);
			uint64_t handle = (uint64_t)*pBuffer;
			vmaAllocation vmaAlloc = { allocation,allocationInfo };
			allocations.putValue(&handle, vmaAlloc);
			
			return bufferCreateResult;
		}
		void vulkanAllocator::destroyBuffer(VkBuffer buffer)
		{
			vmaAllocation bufferAllocation = allocations[(uint64_t)buffer].second;
			//vmaDestroyBuffer(sAllocator, buffer, bufferAllocation.allocation);

		}
	}
}

