#pragma once
#include <core/artemis/device/commandPool.h>
#include <core/artemis/device/buffer.h>
#include <core/artemis/rendering/image.h>
namespace luna 
{
	namespace artemis 
	{
        typedef enum memoryUsage
        {

            UNKNOWN = 0,

            GPU_ONLY = 1,

            CPU_ONLY = 2,

            CPU_TO_GPU = 3,
 
            GPU_TO_CPU = 4,

            CPU_COPY = 5,

            GPU_LAZILY_ALLOCATED = 6,

            AUTO = 7,

            AUTO_PREFER_DEVICE = 8,

            AUTO_PREFER_HOST = 9,

            MAX_ENUM = 0x7FFFFFFF
        } memoryUsage;
		struct vmaAllocation; //forward declare.
		struct allocatorData; //forward declare.
		class allocator
		{
		public:
			buffer& allocateBuffer(const size_t bufferSize,const memoryUsage memUsage,VkBufferUsageFlags bufferUsage);
			image& allocateImage( const VkExtent3D& extent, const uint32_t channels, const VkImageUsageFlags usageFlags, const memoryUsage memoryUsage = GPU_ONLY, const glm::vec4& uv = {0.0f,0.0f,1.0f,1.0f}, bool imageView = true, const VkImageAspectFlags imageAspectFlags = VK_IMAGE_ASPECT_COLOR_BIT);
			void deallocate(const VkBuffer buffer, vmaAllocation* p_allocation);
			void deallocate(const VkImage image, vmaAllocation* p_allocation);

			void* getData(const vmaAllocation* p_allocation);
		protected:
			friend class device;
			allocator(const VkDevice* p_device, const VkInstance* p_instance, const VkPhysicalDevice* p_physicalDevice, const uint32_t apiVersion, const ref<commandPool> transferPool);
		protected:
			friend class buffer;
			size_t getSize(vmaAllocation* allocation);
		private:
			VkFormat getSuitableFormat(const VkImageUsageFlags usageFlags, const uint32_t channels);
			ref<allocatorData> p_data;
		};
	}
}


