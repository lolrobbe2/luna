#include "vulkanIndexBuffer.h"
#include <core/vulkan/utils/vulkanAllocator.h>
namespace luna
{
	namespace vulkan
	{
		vulkanIndexBuffer::vulkanIndexBuffer(uint32_t* indices, uint32_t count)
		{
			size = count;
			utils::vulkanAllocator::createBuffer(&vkIndexBuffer, sizeof(uint32_t) * count,VkBufferUsageFlagBits::VK_BUFFER_USAGE_INDEX_BUFFER_BIT ,VMA_MEMORY_USAGE_CPU_TO_GPU, VMA_ALLOCATION_CREATE_MAPPED_BIT);
			data = utils::vulkanAllocator::getAllocationInfo((uint64_t)vkIndexBuffer).pMappedData;
			memcpy(data, indices, sizeof(uint32_t) * count);
		}

		vulkanIndexBuffer::~vulkanIndexBuffer()
		{
			//utils::vulkanAllocator::destroyBuffer(vkIndexBuffer);
		}

		void vulkanIndexBuffer::bind() const
		{
		}
		void vulkanIndexBuffer::unbind() const
		{
		}
		uint32_t vulkanIndexBuffer::getCount() const
		{
			return size / sizeof(uint32_t);
		}
	}
}
