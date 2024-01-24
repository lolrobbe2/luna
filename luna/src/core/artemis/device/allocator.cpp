#include "allocator.h"
#include <core/vulkan/utils/vma.h>
#include <core/utils/objectStorage.h>

#include <core/debug/debugMacros.h>

namespace luna 
{
	namespace artemis 
	{
		
		typedef struct allocation
		{
			VmaAllocation _allocation;
			VmaAllocationInfo allocationInfo;
			allocation(VmaAllocation allocation, VmaAllocationInfo allocationInfo) : _allocation(allocation),allocationInfo(allocationInfo) {}
		} allocation;
		typedef struct bufferCopy 
		{
			VkBuffer srcBuffer;
			VkBuffer dstBuffer;
		};
		typedef struct imageCopyRegion 
		{
			VkBuffer buffer;
			VkImage image;
			VkImageLayout imageLayout;
		};

		struct allocatorData
		{
			VmaAllocator allocator; //allocator handle.
			std::unordered_map<bufferCopy,std::vector<VkBufferCopy>> bufferRegions;
			std::unordered_map<imageCopyRegion,std::vector<VkBufferImageCopy>> bufferImageRegions; //regions copyBufferToImage
			std::unordered_map<imageCopyRegion,std::vector<VkBufferImageCopy>> imageBufferRegions; //regions copyImageToBuffer

			ref<commandPool> transferPool;
			ref<commandBuffer> commandBuffer;
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
			VmaAllocation _allocation;
			VmaAllocationInfo info;
			VkBuffer _buffer;
			VkResult createRes = vmaCreateBuffer(p_data->allocator, &bufferInfo, &vmaAllocInfo, &_buffer, &_allocation, &info);
			LN_ERR_FAIL_COND_V_MSG(createRes != VK_SUCCESS, buffer(VK_NULL_HANDLE,nullptr,nullptr), "[Artemis] an error occured whilst allocating a buffer, VkResult: " + VK_RESULT(createRes));
			return *(new buffer(_buffer, new allocation(_allocation, info), this));
		}
		image& allocator::allocateImage(const glm::vec2& extent, const uint32_t channels, const VkImageUsageFlags usageFlags, const memoryUsage memoryUsage,const glm::vec4& uv, bool imageView,const VkImageAspectFlags imageAspectFlags)
		{
			VkImageCreateInfo imageCreateInfo = { VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO };

			VkImageFormatProperties properties;

			imageCreateInfo.pNext = nullptr;
			imageCreateInfo.imageType = VK_IMAGE_TYPE_2D;
			VkFormat format = getSuitableFormat(usageFlags, channels);
			imageCreateInfo.format = format;
			VkExtent3D nativeExtent;
			nativeExtent.width = extent.x;
			nativeExtent.height = extent.y;
			nativeExtent.depth = 1.0f;

			imageCreateInfo.extent = nativeExtent;

			imageCreateInfo.mipLevels = 1;
			imageCreateInfo.arrayLayers = 1;
			imageCreateInfo.samples = VK_SAMPLE_COUNT_1_BIT;
			imageCreateInfo.tiling = VK_IMAGE_TILING_OPTIMAL;
			imageCreateInfo.usage = usageFlags;
			imageCreateInfo.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;

			VmaAllocation _allocation;
			VmaAllocationCreateInfo allocationCreateInfo = {};
			allocationCreateInfo.usage = (VmaMemoryUsage)memoryUsage;
			VmaAllocationInfo info;
			VkImage _image;
			VkResult createRes = vmaCreateImage(p_data->allocator, &imageCreateInfo, &allocationCreateInfo,&_image , &_allocation, &info);
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

				LN_ERR_FAIL_COND_V_MSG(createRes != VK_SUCCESS,image(), "[Artemis] an error occured whilst creating imageView!");
				
				return *new image(_image, imageView, new allocation(_allocation, info),extent, format, uv);
			}
			return *new image(_image, new allocation(_allocation, info), extent, format, uv);
		}
		void allocator::deallocate(const VkBuffer buffer,allocation* p_allocation)
		{
			LN_ERR_FAIL_COND_MSG(buffer == VK_NULL_HANDLE, "[Artemis] cannot deallocate a buffer thats has already been detroyed!");
			LN_ERR_FAIL_COND_MSG(p_allocation == nullptr, "[Artemis] allocation was invalid (nullptr)");
			vmaDestroyBuffer(p_data->allocator, buffer, p_allocation->_allocation);
			delete p_allocation; //invalidate p_allocation
		}
		void allocator::deallocate(const VkImage image, allocation* p_allocation)
		{
			LN_ERR_FAIL_COND_MSG(image == VK_NULL_HANDLE, "[Artemis] cannot deallocate a image thats has already been detroyed!");
			LN_ERR_FAIL_COND_MSG(p_allocation == nullptr, "[Artemis] allocation was invalid (nullptr)");
			vmaDestroyImage(p_data->allocator, image, p_allocation->_allocation);
			delete p_allocation; //invalidate p_allocation
		}
		void* allocator::getData(const allocation* p_allocation)
		{
			return p_allocation->_allocation->GetMappedData();
		}
		void allocator::copyBufferToBuffer(const buffer& srcBuffer, const size_t srcOffset, const buffer& dstBuffer, const size_t dstOffset, const size_t size)
		{
			LN_ERR_FAIL_COND_MSG((srcBuffer.getSize() - srcOffset) < size, "[Artemis] could not copyBuffer: srcBuffer size is not large enough");
			LN_ERR_FAIL_COND_MSG((srcBuffer.getSize() - srcOffset) < size, "[Artemis] could not copyBuffer: srcBuffer size is not large enough");

			LN_ERR_FAIL_COND_MSG(srcBuffer, "[Artemis] srcBuffer was invalid (VK_NULL_HANDLE)");
			LN_ERR_FAIL_COND_MSG(dstBuffer, "[Artemis] dstBUffer was invalid (VK_NULL_HANDLE)");

			LN_ERR_FAIL_COND_MSG(p_data->commandBuffer->isRecording(), "[Artemis] commandBuffer is not recording!");

			VkBufferCopy copyInfo;
			copyInfo.dstOffset = dstOffset;
			copyInfo.size = size;
			copyInfo.srcOffset = srcOffset;
			p_data->bufferRegions[{srcBuffer, dstBuffer}].push_back(copyInfo);
		}
		void allocator::copyBufferToBuffer(const buffer& srcBuffer, const buffer& dstBuffer, const size_t dstOffset, const size_t size)
		{
			copyBufferToBuffer(srcBuffer, 0, dstBuffer, dstOffset, size);
		}
		void allocator::copyBufferToBuffer(const buffer& srcBuffer, const size_t srcOffset, const buffer& dstBuffer, const size_t size)
		{
			copyBufferToBuffer(srcBuffer, srcOffset, dstBuffer, 0, size);
		}
		void allocator::copyBufferToBuffer(const buffer& srcBuffer, const buffer& dstBuffer, const size_t size)
		{
			copyBufferToBuffer(srcBuffer, 0, dstBuffer, 0, size);
		}
		void allocator::copyBufferToBuffer(const buffer& srcBuffer, const buffer& dstBuffer)
		{
			LN_ERR_FAIL_COND_MSG(srcBuffer.getSize() != dstBuffer.getSize(), "[Artemis] when direct copying the size of src and dst buffer need to be equal!");
			copyBufferToBuffer(srcBuffer, dstBuffer, srcBuffer.getSize());
		}
		void allocator::copyBufferToImage(const buffer& srcBuffer,const size_t bufferOffset,const glm::vec2& bufferExtent, const image& image)
		{
			LN_ERR_FAIL_COND(srcBuffer, "[Artemis] srcBuffer is invalid (VK_NULL_HANDLE)");
			LN_ERR_FAIL_COND(image, "[Artemis] srcBuffer is invalid (VK_NULL_HANDLE)");


			LN_ERR_FAIL_COND(bufferExtent.x > image.getExtent().x, "[Artemis] bufferExtent.x/bufferRowLength needs to be < or = to image.getExtent().x");
			LN_ERR_FAIL_COND(bufferExtent.y > image.getExtent().y, "^[Artemis] bufferExtent.y/bufferImageHeight needs to be < or = to image.getExtent().y");
			VkBufferImageCopy imageRegion;
			imageRegion.bufferOffset = bufferOffset;
			imageRegion.bufferRowLength = bufferExtent.x;
			imageRegion.bufferImageHeight = bufferExtent.y;

			imageRegion.imageExtent = image;
			imageRegion.imageOffset = image;
			imageRegion.imageSubresource.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
			imageRegion.imageSubresource.mipLevel = 0;
			imageRegion.imageSubresource.baseArrayLayer = 0;
			imageRegion.imageSubresource.layerCount = 1;
			p_data->bufferImageRegions[{srcBuffer.getBuffer(), image.getImage()}].push_back(imageRegion);
		}
		void allocator::copyBufferToImage(const buffer& srcBuffer, const size_t bufferOffset, const image& image)
		{
			copyBufferToImage(srcBuffer, bufferOffset, image, image);
		}
		void allocator::copyBufferToImage(const buffer& srcBuffer, const image& image)
		{
			copyBufferToImage(srcBuffer, 0, image);
		}
		void allocator::copyImageToBuffer(const buffer& srcBuffer, const size_t bufferOffset, const glm::vec2& bufferExtent, const image& image,const VkImageLayout destinationLayout)
		{
			LN_ERR_FAIL_COND(srcBuffer, "[Artemis] srcBuffer is invalid (VK_NULL_HANDLE)");
			LN_ERR_FAIL_COND(image, "[Artemis] srcBuffer is invalid (VK_NULL_HANDLE)");


			LN_ERR_FAIL_COND(bufferExtent.x > image.getExtent().x, "[Artemis] bufferExtent.x/bufferRowLength needs to be < or = to image.getExtent().x");
			LN_ERR_FAIL_COND(bufferExtent.y > image.getExtent().y, "^[Artemis] bufferExtent.y/bufferImageHeight needs to be < or = to image.getExtent().y");
			VkBufferImageCopy imageRegion;
			imageRegion.bufferOffset = bufferOffset;
			imageRegion.bufferRowLength = bufferExtent.x;
			imageRegion.bufferImageHeight = bufferExtent.y;

			imageRegion.imageExtent = image;
			imageRegion.imageOffset = image;
			imageRegion.imageSubresource.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
			imageRegion.imageSubresource.mipLevel = 0;
			imageRegion.imageSubresource.baseArrayLayer = 0;
			imageRegion.imageSubresource.layerCount = 1;
			p_data->imageBufferRegions[{srcBuffer.getBuffer(), image.getImage(),destinationLayout}].push_back(imageRegion);
		}
		void allocator::copyImageToBuffer(const buffer& srcBuffer, const size_t bufferOffset, const image& image, const VkImageLayout destinationLayout)
		{
			copyImageToBuffer(srcBuffer, bufferOffset, image, image,destinationLayout);
		}
		void allocator::copyImageToBuffer(const buffer& srcBuffer, const image& image, const VkImageLayout destinationlayout)
		{
			copyImageToBuffer(srcBuffer, 0, image,destinationlayout);
		}

		void allocator::flush()
		{
			p_data->commandBuffer->begin(0);
			//copyBufferToBuffer
			for (const auto& bufferCopy : p_data->bufferRegions) if(bufferCopy.second.size()) vkCmdCopyBuffer(*p_data->commandBuffer, bufferCopy.first.srcBuffer, bufferCopy.first.dstBuffer, bufferCopy.second.size(), bufferCopy.second.data());
			//copyBufferToImage
			for (const auto& bufferImageRegion : p_data->bufferImageRegions) if (bufferImageRegion.second.size()) vkCmdCopyBufferToImage(*p_data->commandBuffer, bufferImageRegion.first.buffer, bufferImageRegion.first.image, bufferImageRegion.first.imageLayout,bufferImageRegion.second.size(),bufferImageRegion.second.data());
			//copyImageToBuffer
			for (const auto& imageBufferRegion : p_data->imageBufferRegions) if (imageBufferRegion.second.size()) vkCmdCopyBufferToImage(*p_data->commandBuffer, imageBufferRegion.first.buffer, imageBufferRegion.first.image, imageBufferRegion.first.imageLayout, imageBufferRegion.second.size(), imageBufferRegion.second.data());
			p_data->commandBuffer->end();
			p_data->transferPool->flush({ *p_data->commandBuffer }, {}, {}, fence(), nullptr, true);
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
			p_allocatorData->p_device = p_device;
			p_allocatorData->p_instance = p_instance;
			p_allocatorData->p_physicalDevice = p_physicalDevice;
			p_data = ref<allocatorData>(p_allocatorData);
		}
		size_t allocator::getSize(allocation* allocation)
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