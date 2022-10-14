#pragma once
#include <vk_mem_alloc.h>
#include <core/utils/vectorCache.h>
#include <core/rendering/device.h>
namespace luna
{
	namespace utils
	{
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
		private:
			struct allocation
			{
				VmaAllocation allocation;
				uint64_t vulkanHandle;
			};
			vectorCache<allocation> allocationCache;
			inline static VmaAllocator sAllocator;
			VkQueue transferQueue; //for later;
		};
		
	}
}

