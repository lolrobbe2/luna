#pragma once
#include <vk_mem_alloc.h>
#include <core/utils/vectorCache.h>
#include <core/rendering/device.h>
#include <core/utils/objectStorage.h>
namespace luna
{
	namespace utils
	{
		typedef uint64_t allocationHandle;

		class vulkanAllocator
		{
		public:
			/**
			 * @brief allocator constructor.
			 * \param ref<renderer::device> device reference to the device
			 */
			static void init(ref<renderer::device> device);
			/**
			 * @brief destroys the allocator.
			 */
			static void shutdown();
			/**
			 * @brief returns the allocation info from the given handle.
			 * 
			 * \param handle
			 * \return VmaAllocationInfo
			 */
			static VmaAllocationInfo getAllocationInfo(const uint64_t& handle);
			/**
			 * @brief creates a VkImage object.
			 * 
			 * \param pImage pointer to the image handle
			 * \param usageFlags
			 * \param memoryUsage VMA_MEMORY_USAGE
			 * \param extent image extend
			 * \param format VkImageFormat
			 * \return 
			 */
			static VkResult createImage(VkImage* pImage, const VkImageUsageFlags& usageFlags, const VmaMemoryUsage& memoryUsage, const VkExtent3D& extent, const VkFormat& format);
			/**
			 * @brief destroys the VkImage object.
			 * 
			 * \param image
			 * \return 
			 */
			static VkResult destroyImage(const VkImage& image);
			/**
			 * @brief creates a VkImageView object.
			 * 
			 * \param pImageView pointer to imageView handle
			 * \param image VkImage handle
			 * \param format
			 * \param imageAspectFlags
			 * \return 
			 */
			static VkResult createImageView(VkImageView* pImageView, const VkImage& image, const VkFormat& format, const VkImageAspectFlags& imageAspectFlags);
			/**
			 * @brief destroys the VkImageView.
			 * 
			 * \param imageView
			 * \return 
			 */
			static VkResult destroyImageView(const VkImageView& imageView);
			/**
			 * @brief creates a VkBuffer object.
			 * 
			 * \param pBuffer pointer to the buffer handle
			 * \param allocSize 
			 * \param usage
			 * \param memoryUsage
			 * \return 
			 */
			static VkResult createBuffer(VkBuffer* pBuffer, size_t allocSize, VkBufferUsageFlags usage, VmaMemoryUsage memoryUsage);
			/**
			 * @brief destroys the VkBuffer object.
			 * 
			 * \param buffer
			 */
			static void destroyBuffer(VkBuffer buffer);
		private:
			struct vmaAllocation
			{
				VmaAllocation allocation;
				VmaAllocationInfo allocationInfo;
			};
			inline static ref<renderer::device> pDevice; //ref to graphics device.
			inline static VmaAllocator sAllocator; //allocator handle.
			inline static objectStorage<vmaAllocation> allocations;
			VkQueue transferQueue; //for later;

		};
		
	}
}

