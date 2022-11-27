#include "vulkanVertexBuffer.h"
#include <core/vulkan/utils/vulkanAllocator.h>

namespace luna
{
	namespace vulkan
	{
		vulkanVertexBuffer::vulkanVertexBuffer(uint32_t size)
		{
			this->size = size;
			utils::vulkanAllocator::createBuffer(&vkVertexBuffer, size, VK_BUFFER_USAGE_VERTEX_BUFFER_BIT, VMA_MEMORY_USAGE_CPU_TO_GPU);

		}
		vulkanVertexBuffer::vulkanVertexBuffer(const void* data, uint32_t size)
		{
			setData(data, size);
		}
		vulkanVertexBuffer::~vulkanVertexBuffer()
		{
			utils::vulkanAllocator::destroyBuffer(vkVertexBuffer);
		}
		void vulkanVertexBuffer::bind() const
		{	

		}
		void vulkanVertexBuffer::unbind() const
		{

		}
		void vulkanVertexBuffer::setData(const void* data, uint32_t size)
		{
			this->size = size;
			utils::vulkanAllocator::createBuffer(&vkVertexBuffer, size, VK_BUFFER_USAGE_VERTEX_BUFFER_BIT, VMA_MEMORY_USAGE_CPU_TO_GPU);
			this->data = utils::vulkanAllocator::getAllocationInfo((uint64_t)vkVertexBuffer).pMappedData;
			memcpy(this->data, data, size);
		}

	}
}

