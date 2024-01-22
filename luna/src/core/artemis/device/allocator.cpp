#include "allocator.h"
#include <core/utils/objectStorage.h>
#include <vk_mem_alloc.h>
#include <core/debug/debugMacros.h>

namespace luna 
{
	namespace artemis 
	{
		
		struct vmaAllocation
		{
			VmaAllocation allocation;
			VmaAllocationInfo allocationInfo;
			vmaAllocation(VmaAllocation allocation, VmaAllocationInfo allocationInfo) : allocation(allocation),allocationInfo(allocationInfo) {}
		};
		struct transferCommand
		{
			uint64_t bufferOffset;
			VkBuffer sourceBuffer;
			VkImage VulkanImage;
			VkFormat ImageFormat;
			glm::vec2 subImageHeight;
			glm::vec3 dimensions;
			glm::vec3 offset;
		};
		struct allocatorData
		{
			VmaAllocator allocator; //allocator handle.
			std::vector<transferCommand> transferCommands; //TODO make attomic var
			ref<commandPool> transferPool;
			const VkDevice* p_device;
			const VkInstance* p_instance;
			const VkPhysicalDevice* p_physicalDevice;
		};
		buffer& allocator::allocateBuffer(const size_t bufferSize, const memoryUsage memUsage, VkBufferUsageFlags bufferUsage)
		{
			VkBufferCreateInfo bufferInfo = {};
			bufferInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
			
			bufferInfo.size = bufferSize;
			bufferInfo.usage = bufferUsage;

			VmaAllocationCreateInfo vmaAllocInfo = {};
			vmaAllocInfo.usage = (VmaMemoryUsage)memUsage;
			VmaAllocation allocation;
			VmaAllocationInfo info;
			VkBuffer _buffer;
			VkResult createRes = vmaCreateBuffer(p_data->allocator, &bufferInfo, &vmaAllocInfo, &_buffer, &allocation, &info);
			LN_ERR_FAIL_COND_V_MSG(createRes != VK_SUCCESS, buffer(VK_NULL_HANDLE,nullptr,nullptr), "[Artemis] an error occured whilst allocating a buffer, VkResult: " + VK_RESULT(createRes));
			return *(new buffer(_buffer, new vmaAllocation(allocation, info), this));
		}
		image& allocator::allocateImage(const VkExtent3D& extent, const uint32_t channels, const VkImageUsageFlags usageFlags, const memoryUsage memoryUsage,const glm::vec4& uv, bool imageView,const VkImageAspectFlags imageAspectFlags)
		{
			VkImageCreateInfo imageCreateInfo = { VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO };

			VkImageFormatProperties properties;

			imageCreateInfo.pNext = nullptr;
			imageCreateInfo.imageType = VK_IMAGE_TYPE_2D;
			VkFormat format = getSuitableFormat(usageFlags, channels);
			imageCreateInfo.format = format;
			imageCreateInfo.extent = extent;

			imageCreateInfo.mipLevels = 1;
			imageCreateInfo.arrayLayers = 1;
			imageCreateInfo.samples = VK_SAMPLE_COUNT_1_BIT;
			imageCreateInfo.tiling = VK_IMAGE_TILING_OPTIMAL;
			imageCreateInfo.usage = usageFlags;
			imageCreateInfo.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;

			VmaAllocation allocation;
			VmaAllocationCreateInfo allocationCreateInfo = {};
			allocationCreateInfo.usage = VMA_MEMORY_USAGE_AUTO;
			VmaAllocationInfo info;
			VkImage _image;
			VkResult createRes = vmaCreateImage(p_data->allocator, &imageCreateInfo, &allocationCreateInfo,&_image , &allocation, &info);
			LN_ERR_FAIL_COND_V_MSG(createRes != VK_SUCCESS, image(), "[Artemis] an error occured during image creation, VkResult: " + VK_RESULT(createRes));
			
			if(imageView)
			{
				VkImageView imageView;
				VkImageViewCreateInfo imageViewCreateInfo = { VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO };
				imageViewCreateInfo.pNext = nullptr;

				imageViewCreateInfo.viewType = VK_IMAGE_VIEW_TYPE_2D;
				imageViewCreateInfo.image = _image;
				imageViewCreateInfo.format = format;
				imageViewCreateInfo.subresourceRange.baseMipLevel = 0;
				imageViewCreateInfo.subresourceRange.levelCount = 1;
				imageViewCreateInfo.subresourceRange.baseArrayLayer = 0;
				imageViewCreateInfo.subresourceRange.layerCount = 1;
				imageViewCreateInfo.subresourceRange.aspectMask = imageAspectFlags;
				VkResult createRes = vkCreateImageView(*p_data->p_device, &imageViewCreateInfo, nullptr, &imageView);

				LN_ERR_FAIL_COND_MSG(createRes != VK_SUCCESS, "[Artemis] an error occured whilst creating imageView!");
				
				return *new image(_image, imageView, new vmaAllocation(allocation, info),{extent.width,extent.height}, format, uv);
			}
			return *new image(_image, new vmaAllocation(allocation, info), { extent.width,extent.height }, format, uv);
		}
		void allocator::deallocate(const VkBuffer buffer,vmaAllocation* p_allocation)
		{
			LN_ERR_FAIL_COND_MSG(buffer == VK_NULL_HANDLE, "[Artemis] cannot deallocate a buffer thats has already been detroyed!");
			LN_ERR_FAIL_COND_MSG(p_allocation == nullptr, "[Artemis] allocation was invalid (nullptr)");
			vmaDestroyBuffer(p_data->allocator, buffer, p_allocation->allocation);
			delete p_allocation; //invalidate p_allocation
		}
		void allocator::deallocate(const VkImage image, vmaAllocation* p_allocation)
		{
			LN_ERR_FAIL_COND_MSG(image == VK_NULL_HANDLE, "[Artemis] cannot deallocate a image thats has already been detroyed!");
			LN_ERR_FAIL_COND_MSG(p_allocation == nullptr, "[Artemis] allocation was invalid (nullptr)");
			vmaDestroyImage(p_data->allocator, image, p_allocation->allocation);
			delete p_allocation; //invalidate p_allocation
		}
		void* allocator::getData(const vmaAllocation* p_allocation)
		{
			return p_allocation->allocation->GetMappedData();
		}
		allocator::allocator(const VkDevice* p_device, const VkInstance* p_instance, const VkPhysicalDevice* p_physicalDevice, const uint32_t apiVersion,const ref<commandPool> transferPool)
		{
			LN_ERR_FAIL_COND_MSG(transferPool == nullptr, "[Artemis] transferPool cann not be nullPointer!");
			allocatorData* p_allocatorData = new allocatorData();
			VmaAllocatorCreateInfo allocatorCreateInfo;
			allocatorCreateInfo.instance = *p_instance;
			allocatorCreateInfo.physicalDevice = *p_physicalDevice;
			allocatorCreateInfo.device = *p_device;

			allocatorCreateInfo.pAllocationCallbacks = nullptr;
			allocatorCreateInfo.pDeviceMemoryCallbacks = nullptr;
			allocatorCreateInfo.pVulkanFunctions = nullptr;

			allocatorCreateInfo.pTypeExternalMemoryHandleTypes = nullptr;
			allocatorCreateInfo.pHeapSizeLimit = nullptr;
			allocatorCreateInfo.preferredLargeHeapBlockSize = 0;

			VkPhysicalDeviceMemoryProperties memoryProperties;
			vkGetPhysicalDeviceMemoryProperties(*p_physicalDevice, &memoryProperties);
			allocatorCreateInfo.vulkanApiVersion = apiVersion;

			VkResult createRes = vmaCreateAllocator(&allocatorCreateInfo, &p_allocatorData->allocator);
			LN_ERR_FAIL_COND_MSG(createRes != VK_SUCCESS, "[Artemis] an error occured during allocator creation");
			
			p_allocatorData->transferPool = transferPool;
			p_data = ref<allocatorData>(p_allocatorData);
		}
		size_t allocator::getSize(vmaAllocation* allocation)
		{
			return allocation->allocationInfo.size;
		}
		VkFormat allocator::getSuitableFormat(const VkImageUsageFlags usageFlags, const uint32_t channels)
		{
		
			VkFormat baseFormat = (VkFormat)(9 + channels * 7);
			VkFormat indexedFormat = baseFormat;
			for (size_t i = 0; i < 7; i++)
			{
				VkImageFormatProperties properties;
				VkResult result = vkGetPhysicalDeviceImageFormatProperties(*p_data->p_physicalDevice, indexedFormat, VK_IMAGE_TYPE_2D, VK_IMAGE_TILING_OPTIMAL, usageFlags, 0, &properties);
				if (result == VK_SUCCESS) return indexedFormat;
				indexedFormat = (VkFormat)(indexedFormat + 1);
			}
			return VK_FORMAT_UNDEFINED;
		}
	}
}