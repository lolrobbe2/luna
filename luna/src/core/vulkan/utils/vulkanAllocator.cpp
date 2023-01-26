#include "vulkanAllocator.h"
#include <core/vulkan/device/vulkanDevice.h>
namespace luna
{
	namespace utils
	{
		void vulkanAllocator::init(ref<renderer::device> device)
		{
			LN_PROFILE_FUNCTION();
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

			transferQueue = vDevice->getQueue(vkb::QueueType::graphics);

			vulkan::vulkanCmdPoolSpec commandPoolSpec;
			commandPoolSpec.device = handles.device;
			commandPoolSpec.queueFamilyIndex = vDevice->getQueueIndex(vkb::QueueType::graphics);
			commandPoolSpec.flags = VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT;
			commandPool = ref<vulkan::vulkanCmdPool>(new vulkan::vulkanCmdPool(commandPoolSpec));

			LN_CORE_INFO("vma allocator creation result = {0}", vmaCreateAllocator(&allocatorCreateInfo, &sAllocator));
			pDevice = device;
		}
		void vulkanAllocator::shutdown()
		{
			LN_PROFILE_FUNCTION();
			if (sAllocator == VK_NULL_HANDLE) return;
			vmaDestroyAllocator(sAllocator);
		}

		VmaAllocationInfo vulkanAllocator::getAllocationInfo(const uint64_t& handle)
		{
			LN_PROFILE_FUNCTION();
			return allocations.getValue(handle, vmaAllocation()).second.allocationInfo;
		}

		VkResult vulkanAllocator::createImage(VkImage* pImage, const VkImageUsageFlags& usageFlags, const VmaMemoryUsage& memoryUsage, const VkExtent3D& extent, const VkFormat& format)
		{
			LN_PROFILE_FUNCTION();
			ref<vulkan::vulkanDevice> device = std::dynamic_pointer_cast<vulkan::vulkanDevice>(pDevice);
			VkImageCreateInfo imageCreateInfo = { VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO };
			imageCreateInfo.pNext = nullptr;

			imageCreateInfo.imageType = VK_IMAGE_TYPE_2D;

			imageCreateInfo.format = format;
			imageCreateInfo.extent = extent;

			imageCreateInfo.mipLevels = 1;
			imageCreateInfo.arrayLayers = 1;
			imageCreateInfo.samples = VK_SAMPLE_COUNT_1_BIT;
			if(format == VK_FORMAT_R8G8B8A8_UNORM) imageCreateInfo.tiling = VK_IMAGE_TILING_OPTIMAL;
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
			LN_PROFILE_FUNCTION();
			auto result = allocations.getValue((uint64_t)image,vmaAllocation());
			if (result.first == storageOpSucces) vmaDestroyImage(sAllocator, image, result.second.allocation);
			allocations.eraseValue((uint64_t)image);
			return VK_SUCCESS; //TODO return appropriate VkResult based on object storage rsult.
		}

		VkResult vulkanAllocator::createImageView(VkImageView* pImageView, const VkImage& image, const VkFormat& format, const VkImageAspectFlags& imageAspectFlags)
		{
			LN_PROFILE_FUNCTION();
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
			LN_PROFILE_FUNCTION();
			ref<vulkan::vulkanDevice> device = std::dynamic_pointer_cast<vulkan::vulkanDevice>(pDevice);
			vkDestroyImageView(device->getDeviceHandles().device, imageView, nullptr);
			return VK_SUCCESS;
		}
		VkResult vulkanAllocator::createBuffer(VkBuffer* pBuffer,const size_t& allocSize,const VkBufferUsageFlags& usage,const VmaMemoryUsage& memoryUsage,const VmaAllocationCreateFlags& allocFlags)
		{
			LN_PROFILE_FUNCTION();
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
		void vulkanAllocator::destroyBuffer(const VkBuffer& buffer)
		{
			LN_PROFILE_FUNCTION();
			ref<vulkan::vulkanDevice> vDevice = std::dynamic_pointer_cast<vulkan::vulkanDevice>(pDevice);
			vkDeviceWaitIdle(vDevice->getDeviceHandles().device);
			vmaAllocation bufferAllocation = allocations[(uint64_t)buffer].second;
			vmaDestroyBuffer(sAllocator, buffer, bufferAllocation.allocation);

		}
		void vulkanAllocator::uploadTexture(const VkBuffer& buffer, const VkImage& image,const VkFormat& imageFormat,const glm::vec3& imageDimensions)
		{
			transferCommands.push_back({ buffer,image,imageFormat,imageDimensions });
		}
		void vulkanAllocator::flush()
		{
			LN_PROFILE_FUNCTION();
			if (transferCommands.empty()) return; //when no textures need uploading return.

			vulkan::virtualCmdBuffer commandBuffer;
			commandPool->createNewBuffer(&commandBuffer, 1, VK_COMMAND_BUFFER_LEVEL_PRIMARY);
			commandPool->begin(commandBuffer, VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT);
			std::vector<VkBufferImageCopy> regions;
			regions.resize(transferCommands.size());
			uint32_t i = 0;
			for (transferCommand command : transferCommands)
			{
				
				regions[i].bufferOffset = 0;
				regions[i].bufferRowLength = 0;
				regions[i].bufferImageHeight = 0;

				regions[i].imageExtent = { (unsigned int)command.dimensions.x ,(unsigned int)command.dimensions.y ,1 };
				regions[i].imageOffset = { 0,0,0 };
			
				regions[i].imageSubresource.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
				regions[i].imageSubresource.mipLevel = 0;
				regions[i].imageSubresource.baseArrayLayer = 0;
				regions[i].imageSubresource.layerCount = 1;
				transitionImageLayout(command.VulkanImage, command.ImageFormat , VK_IMAGE_LAYOUT_UNDEFINED, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, commandBuffer);
				vkCmdCopyBufferToImage(commandPool->operator=(commandBuffer), command.sourceBuffer, command.VulkanImage, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, 1,&regions[i]);
				transitionImageLayout(command.VulkanImage, command.ImageFormat, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL, commandBuffer);
				i++;
			}

			commandPool->end(commandBuffer);
			vulkan::commandPoolSubmitInfo submitInfo;
			submitInfo.commandBufferCount = 1;
			submitInfo.pCommandBuffers = &commandBuffer;
			submitInfo.pSignalSemaphores = nullptr;
			submitInfo.pWaitDstStageMask = nullptr;
			submitInfo.pWaitSemaphores = nullptr;
			submitInfo.signalSemaphoreCount = 0;
			submitInfo.waitSemaphoreCount = 0;
			submitInfo.pNext = nullptr;
			commandPool->flush(transferQueue, 1, &submitInfo, VK_NULL_HANDLE); 
			commandPool->freeCommandBuffer(&commandBuffer, 1); 
			for (transferCommand command : transferCommands) destroyBuffer(command.sourceBuffer);
			transferCommands.clear();
		
		}
		void vulkanAllocator::transitionImageLayout(const VkImage& image,const VkFormat& format,const VkImageLayout& oldLayout,const VkImageLayout& newLayout,const vulkan::virtualCmdBuffer& commandBufffer)
		{
			LN_PROFILE_FUNCTION();
			VkImageMemoryBarrier barrier{};
			barrier.sType = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER;
			barrier.oldLayout = oldLayout;
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
			if (oldLayout == VK_IMAGE_LAYOUT_UNDEFINED && newLayout == VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL) 
			{
				barrier.srcAccessMask = VK_ACCESS_NONE;
				barrier.dstAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT;

				sourceStage = VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT;
				destinationStage = VK_PIPELINE_STAGE_TRANSFER_BIT;
			}
			else if (oldLayout == VK_IMAGE_LAYOUT_PRESENT_SRC_KHR && newLayout == VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL) 
			{
				barrier.srcAccessMask = VK_ACCESS_MEMORY_READ_BIT;
				barrier.dstAccessMask = VK_ACCESS_TRANSFER_READ_BIT;

				sourceStage = VK_PIPELINE_STAGE_TRANSFER_BIT;
				destinationStage = VK_PIPELINE_STAGE_TRANSFER_BIT;
			}
			else if (oldLayout == VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL && newLayout == VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL)
			{
				barrier.srcAccessMask = VK_ACCESS_TRANSFER_READ_BIT;
				barrier.dstAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT;

				sourceStage = VK_PIPELINE_STAGE_TRANSFER_BIT;
				destinationStage = VK_PIPELINE_STAGE_TRANSFER_BIT;
			}
			else if (oldLayout == VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL && newLayout == VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL) 
			{
				barrier.srcAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT;
				barrier.dstAccessMask = VK_ACCESS_SHADER_READ_BIT;

				sourceStage = VK_PIPELINE_STAGE_TRANSFER_BIT;
				destinationStage = VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT;
			}
			else if (oldLayout == VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL && newLayout == VK_IMAGE_LAYOUT_PRESENT_SRC_KHR)
			{
				barrier.srcAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT;
				barrier.dstAccessMask = VK_ACCESS_INDIRECT_COMMAND_READ_BIT;

				sourceStage = VK_PIPELINE_STAGE_TRANSFER_BIT;
				destinationStage = VK_PIPELINE_STAGE_DRAW_INDIRECT_BIT;
			}
			else if (oldLayout == VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL && newLayout == VK_IMAGE_LAYOUT_UNDEFINED)
			{
				barrier.srcAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT;
				barrier.dstAccessMask = VK_ACCESS_NONE;

				sourceStage = VK_PIPELINE_STAGE_TRANSFER_BIT;
				destinationStage = VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT;
			}
			else LN_CORE_ERROR("incorrect layout = {0}", newLayout);
			

			vkCmdPipelineBarrier(
				commandPool->operator=(commandBufffer),
				sourceStage, destinationStage,
				0,
				0, nullptr,
				0, nullptr,
				1, &barrier);

		}
	}
}

