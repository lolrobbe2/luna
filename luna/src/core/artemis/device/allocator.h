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
		struct allocatorData; //forward declare.
		class allocator
		{
		public:
			buffer& allocateBuffer(const size_t bufferSize,const memoryUsage memUsage,VkBufferUsageFlags bufferUsage);
			image& allocateImage( const glm::vec2& extent, const uint32_t channels, const VkImageUsageFlags usageFlags, const memoryUsage memoryUsage = GPU_ONLY, const glm::vec4& uv = {0.0f,0.0f,1.0f,1.0f}, bool imageView = true, const VkImageAspectFlags imageAspectFlags = VK_IMAGE_ASPECT_COLOR_BIT);
			void deallocate(const VkBuffer buffer, allocation* p_allocation);
			void deallocate(const VkImage image, allocation* p_allocation);

			void* getData(const allocation* p_allocation);

			/**
			* @brief copies on buffer to another.
			* @brief copuBUfferToBUffer commands are guaranteed to be executed in the order they are called!.
			*/
			void copyBufferToBuffer(const buffer& srcBuffer, const size_t srcOffset, const buffer& dstBuffer, const size_t dstOffset, const size_t size);
			void copyBufferToBuffer(const buffer& srcBuffer, const buffer& dstBuffer, const size_t dstOffset, const size_t size);
			void copyBufferToBuffer(const buffer& srcBuffer, const size_t srcOffset, const buffer& dstBuffer, const size_t size);
			void copyBufferToBuffer(const buffer& srcBuffer, const buffer& dstBuffer,const size_t size);
			void copyBufferToBuffer(const buffer& srcBuffer, const buffer& dstBuffer);

			void copyBufferToImage(const buffer& srcBuffer,const image& image);

		protected:
			friend class device;
			allocator(const VkDevice* p_device, const VkInstance* p_instance, const VkPhysicalDevice* p_physicalDevice, const uint32_t apiVersion, const ref<commandPool> transferPool);
		protected:
			friend class buffer;
			size_t getSize(allocation* allocation);
		private:
			VkFormat getSuitableFormat(const VkImageUsageFlags usageFlags, const uint32_t channels);
			ref<allocatorData> p_data;
		};
	}
}


