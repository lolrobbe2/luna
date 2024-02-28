#include "allocator.h"
#include <core/vulkan/utils/vma.h>
#include <core/utils/objectStorage.h>

#include <core/debug/debugMacros.h>
#include <functional>
namespace luna
{
	namespace artemis
	{
		struct bufferCopy
		{
			VkBuffer srcBuffer;
			VkBuffer dstBuffer;
			bool operator==(bufferCopy b) const
			{
				return std::tie(srcBuffer, dstBuffer) == std::tie(b.srcBuffer, b.dstBuffer);
			}
		};
		struct bufferComp
		{

			bool operator() (bufferCopy a, bufferCopy b) const
			{
				return std::tie(a.srcBuffer, a.dstBuffer) > std::tie(b.srcBuffer, b.dstBuffer);
			}
		};
		struct imageCopyRegion
		{
			VkBuffer buffer;
			VkImage image;
			VkImageLayout imageLayout;
			bool operator== (imageCopyRegion B) const
			{
				return std::tie(buffer, image, imageLayout) > std::tie(B.buffer, B.image, B.imageLayout);
			}
		};

		struct imageCopyComp
		{
			bool operator() (imageCopyRegion A, imageCopyRegion B) const
			{
				return std::tie(A.buffer, A.image, A.imageLayout) > std::tie(B.buffer, B.image, B.imageLayout);
			}
		};
		struct pipelinBarrieStageFLags
		{
			VkPipelineStageFlags srcStage;
			VkPipelineStageFlags dstStage;
			bool operator==(pipelinBarrieStageFLags B) const
			{
				return std::tie(srcStage, dstStage) == std::tie(B.srcStage, B.dstStage);
			}
		};

	}
}
template <>
struct std::hash<luna::artemis::imageCopyRegion>
{
	std::size_t operator()(const luna::artemis::imageCopyRegion& k) const
	{
		return ((hash<uint64_t>()((uint64_t)k.buffer)
			^ (hash<uint64_t>()((uint64_t)k.image) << 1)) >> 1)
			^ (hash<uint64_t>()(k.imageLayout) << 1);
	}
};
template <>
struct std::hash<luna::artemis::bufferCopy>
{
	std::size_t operator()(const luna::artemis::bufferCopy& k) const
	{
		return ((hash<uint64_t>()((uint64_t)k.srcBuffer)
			^ (hash<uint64_t>()((uint64_t)k.dstBuffer) << 1)) >> 1);
	}
};
template <>
struct std::hash<luna::artemis::pipelinBarrieStageFLags>
{
	std::size_t operator()(const luna::artemis::pipelinBarrieStageFLags& k) const
	{
		return ((hash<uint64_t>()((uint64_t)k.srcStage)
			^ (hash<uint64_t>()((uint64_t)k.dstStage) << 1)) >> 1);
	}
};
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

		struct allocatorData
		{
	
			VmaAllocator allocator; //allocator handle.
			std::unordered_map<pipelinBarrieStageFLags,std::vector<VkImageMemoryBarrier>> frontBarriers;
			std::unordered_map<bufferCopy,std::vector<VkBufferCopy>> bufferRegions;
			std::unordered_map<imageCopyRegion,std::vector<VkBufferImageCopy>> bufferImageRegions; //regions copyBufferToImage
			std::unordered_map<imageCopyRegion,std::vector<VkBufferImageCopy>> imageBufferRegions; //regions copyImageToBuffer
			std::unordered_map<pipelinBarrieStageFLags, std::vector<VkImageMemoryBarrier>> backBarriers;

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
			vmaAllocInfo.flags = memUsage == CPU_TO_GPU || CPU_ONLY || CPU_COPY ? VMA_ALLOCATION_CREATE_HOST_ACCESS_RANDOM_BIT | VMA_ALLOCATION_CREATE_MAPPED_BIT : VMA_ALLOCATION_CREATE_STRATEGY_MIN_TIME_BIT;
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
			p_data->bufferImageRegions[{srcBuffer.getBuffer(), image.getImage(), VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL}].push_back(imageRegion);
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
		void allocator::transitionImageLayoutFront(image& image,const VkImageLayout currentLayout,const VkImageLayout newLayout)
		{
			VkImageMemoryBarrier barrier{};
			barrier.sType = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER;
			barrier.oldLayout = currentLayout;
			barrier.newLayout = newLayout;
			barrier.srcQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
			barrier.dstQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
			barrier.image = image;
			barrier.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
			barrier.subresourceRange.baseMipLevel = 0;
			barrier.subresourceRange.levelCount = 1;
			barrier.subresourceRange.baseArrayLayer = 0;
			barrier.subresourceRange.layerCount = 1;

			VkPipelineStageFlags sourceStage = VK_PIPELINE_STAGE_NONE;
			VkPipelineStageFlags destinationStage = VK_PIPELINE_STAGE_NONE;
			if (currentLayout == VK_IMAGE_LAYOUT_UNDEFINED && newLayout == VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL)
			{
				barrier.srcAccessMask = VK_ACCESS_NONE;
				barrier.dstAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT;

				sourceStage = VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT;
				destinationStage = VK_PIPELINE_STAGE_TRANSFER_BIT;
			}
			else if (currentLayout == VK_IMAGE_LAYOUT_PRESENT_SRC_KHR && newLayout == VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL)
			{
				barrier.srcAccessMask = VK_ACCESS_MEMORY_READ_BIT;
				barrier.dstAccessMask = VK_ACCESS_TRANSFER_READ_BIT;

				sourceStage = VK_PIPELINE_STAGE_TRANSFER_BIT;
				destinationStage = VK_PIPELINE_STAGE_TRANSFER_BIT;
			}
			else if (currentLayout == VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL && newLayout == VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL)
			{
				barrier.srcAccessMask = VK_ACCESS_TRANSFER_READ_BIT;
				barrier.dstAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT;

				sourceStage = VK_PIPELINE_STAGE_TRANSFER_BIT;
				destinationStage = VK_PIPELINE_STAGE_TRANSFER_BIT;
			}
			else if (currentLayout == VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL && newLayout == VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL)
			{
				barrier.srcAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT;
				barrier.dstAccessMask = VK_ACCESS_SHADER_READ_BIT;

				sourceStage = VK_PIPELINE_STAGE_TRANSFER_BIT;
				destinationStage = VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT;
			}
			else if (currentLayout == VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL && newLayout == VK_IMAGE_LAYOUT_PRESENT_SRC_KHR)
			{
				barrier.srcAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT;
				barrier.dstAccessMask = VK_ACCESS_INDIRECT_COMMAND_READ_BIT;

				sourceStage = VK_PIPELINE_STAGE_TRANSFER_BIT;
				destinationStage = VK_PIPELINE_STAGE_DRAW_INDIRECT_BIT;
			}
			else if (currentLayout == VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL && newLayout == VK_IMAGE_LAYOUT_UNDEFINED)
			{
				barrier.srcAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT;
				barrier.dstAccessMask = VK_ACCESS_NONE;

				sourceStage = VK_PIPELINE_STAGE_TRANSFER_BIT;
				destinationStage = VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT;
			}
			p_data->frontBarriers[{sourceStage,destinationStage}].push_back(barrier);
		}
		void allocator::transitionImageLayoutBack(image& image,const VkImageLayout currentLayout, const VkImageLayout newLayout)
		{
			VkImageMemoryBarrier barrier{};
			barrier.sType = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER;
			barrier.oldLayout = currentLayout;
			barrier.newLayout = newLayout;
			barrier.srcQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
			barrier.dstQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
			barrier.image = image;
			barrier.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
			barrier.subresourceRange.baseMipLevel = 0;
			barrier.subresourceRange.levelCount = 1;
			barrier.subresourceRange.baseArrayLayer = 0;
			barrier.subresourceRange.layerCount = 1;

			VkPipelineStageFlags sourceStage = VK_PIPELINE_STAGE_NONE;
			VkPipelineStageFlags destinationStage = VK_PIPELINE_STAGE_NONE;
			//switch statement?
			if (currentLayout == VK_IMAGE_LAYOUT_UNDEFINED && newLayout == VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL)
			{
				barrier.srcAccessMask = VK_ACCESS_NONE;
				barrier.dstAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT;

				sourceStage = VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT;
				destinationStage = VK_PIPELINE_STAGE_TRANSFER_BIT;
			}
			else if (currentLayout == VK_IMAGE_LAYOUT_PRESENT_SRC_KHR && newLayout == VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL)
			{
				barrier.srcAccessMask = VK_ACCESS_MEMORY_READ_BIT;
				barrier.dstAccessMask = VK_ACCESS_TRANSFER_READ_BIT;

				sourceStage = VK_PIPELINE_STAGE_TRANSFER_BIT;
				destinationStage = VK_PIPELINE_STAGE_TRANSFER_BIT;
			}
			else if (currentLayout == VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL && newLayout == VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL)
			{
				barrier.srcAccessMask = VK_ACCESS_TRANSFER_READ_BIT;
				barrier.dstAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT;

				sourceStage = VK_PIPELINE_STAGE_TRANSFER_BIT;
				destinationStage = VK_PIPELINE_STAGE_TRANSFER_BIT;
			}
			else if (currentLayout == VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL && newLayout == VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL)
			{
				barrier.srcAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT;
				barrier.dstAccessMask = VK_ACCESS_SHADER_READ_BIT;

				sourceStage = VK_PIPELINE_STAGE_TRANSFER_BIT;
				destinationStage = VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT;
			}
			else if (currentLayout == VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL && newLayout == VK_IMAGE_LAYOUT_PRESENT_SRC_KHR)
			{
				barrier.srcAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT;
				barrier.dstAccessMask = VK_ACCESS_INDIRECT_COMMAND_READ_BIT;

				sourceStage = VK_PIPELINE_STAGE_TRANSFER_BIT;
				destinationStage = VK_PIPELINE_STAGE_DRAW_INDIRECT_BIT;
			}
			else if (currentLayout == VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL && newLayout == VK_IMAGE_LAYOUT_UNDEFINED)
			{
				barrier.srcAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT;
				barrier.dstAccessMask = VK_ACCESS_NONE;

				sourceStage = VK_PIPELINE_STAGE_TRANSFER_BIT;
				destinationStage = VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT;
			}
			p_data->backBarriers[{sourceStage, destinationStage}].push_back(barrier);
		}
		void allocator::flush()
		{
			LN_PROFILE_FUNCTION();
			p_data->commandBuffer->begin(0);
			//fronBarriers
			if (p_data->frontBarriers.size()) for (const auto& barriers : p_data->frontBarriers)vkCmdPipelineBarrier(*p_data->commandBuffer, barriers.first.srcStage, barriers.first.dstStage, 0, 0, nullptr, 0, nullptr, barriers.second.size(), barriers.second.data());
			//copyBufferToBuffer
			for (const auto& bufferCopy : p_data->bufferRegions) if(bufferCopy.second.size()) vkCmdCopyBuffer(*p_data->commandBuffer, bufferCopy.first.srcBuffer, bufferCopy.first.dstBuffer, bufferCopy.second.size(), bufferCopy.second.data());
			//copyBufferToImage
			for (const auto& bufferImageRegion : p_data->bufferImageRegions) if (bufferImageRegion.second.size()) vkCmdCopyBufferToImage(*p_data->commandBuffer, bufferImageRegion.first.buffer, bufferImageRegion.first.image, bufferImageRegion.first.imageLayout,bufferImageRegion.second.size(),bufferImageRegion.second.data());
			//copyImageToBuffer
			for (const auto& imageBufferRegion : p_data->imageBufferRegions) if (imageBufferRegion.second.size()) vkCmdCopyBufferToImage(*p_data->commandBuffer, imageBufferRegion.first.buffer, imageBufferRegion.first.image, imageBufferRegion.first.imageLayout, imageBufferRegion.second.size(), imageBufferRegion.second.data());
			//backBarriers
			if (p_data->backBarriers.size()) for (const auto& barriers : p_data->backBarriers)vkCmdPipelineBarrier(*p_data->commandBuffer, barriers.first.srcStage, barriers.first.dstStage, 0, 0, nullptr, 0, nullptr, barriers.second.size(), barriers.second.data());

			p_data->commandBuffer->end();
			p_data->transferPool->flush({ p_data->commandBuffer.get()}, {}, {}, fence(), nullptr, true);
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
			p_allocatorData->commandBuffer = p_allocatorData->transferPool->getCommandBuffer();
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

