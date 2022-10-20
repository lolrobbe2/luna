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
			static VkResult createImage(VkImage* pImage, const VkImageUsageFlags& usageFlags, const VmaMemoryUsage& memoryUsage, const VkExtent3D& extent, const VkFormat& format);
			static VkResult destroyImage(const VkImage& image);
			static VkResult createImageView(VkImageView* pImageView, const VkImage& image, const VkFormat& format, const VkImageAspectFlags& imageAspectFlags);
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

