#pragma once
#include <vk_mem_alloc.h>
#include <core/utils/vectorCache.h>
#include <core/rendering/device.h>
namespace luna
{
	namespace utils
	{
		typedef uint64_t allocationHandle;

		class vulkanAllocator
		{
		public:
			/**
			 * @brief base allocator constructor.
			 */
			vulkanAllocator(){};
			/**
			 * @brief allocator constructor.
			 * \param ref<renderer::device> device reference to the device
			 */
			vulkanAllocator(ref<renderer::device> device);
			/**
			 * @brief destroys the allocator.
			 */
			virtual ~vulkanAllocator();
			VkResult createImage(VkImage* pImage, VkImageUsageFlags usageFlags, VmaMemoryUsage memoryUsage, VkExtent3D extent, VkFormat format);
		private:
			struct vmaAllocation
			{
				VmaAllocation allocation;
				VmaAllocationInfo allocationInfo;
			};
			inline static ref<renderer::device> pDevice; //ref to graphics device.
			inline static VmaAllocator sAllocator;
			VkQueue transferQueue; //for later;7

		};
		
	}
}

