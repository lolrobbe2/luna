#pragma once
#include <core/artemis/device/commandPool.h>
#include <core/artemis/device/buffer.h>
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
			image& allocateImage();
			void deallocate(const VkBuffer buffer, vmaAllocation* p_allocation);
			void deallocate(const VkImage image, vmaAllocation* p_allocation);
		protected:
			friend class device;
			allocator(const VkDevice* p_device, const VkInstance* p_instance, const VkPhysicalDevice* p_physicalDevice, const uint32_t apiVersion, const ref<commandPool> transferPool);
		protected:
			friend class buffer;
			size_t getSize(vmaAllocation* allocation);
		private:
			ref<allocatorData> p_data;
		};
	}
}


