#include "vulkanVertexBuffer.h"
#include <core/vulkan/utils/vulkanAllocator.h>

namespace luna
{
	namespace vulkan
	{
		vulkanVertexBuffer::vulkanVertexBuffer(uint32_t size)
		{
			LN_PROFILE_FUNCTION();
			this->size = size;
			utils::vulkanAllocator::createBuffer(&vkVertexBuffer, size, VK_BUFFER_USAGE_VERTEX_BUFFER_BIT, VMA_MEMORY_USAGE_CPU_TO_GPU,VMA_ALLOCATION_CREATE_MAPPED_BIT);
			data = utils::vulkanAllocator::getAllocationInfo((uint64_t)vkVertexBuffer).pMappedData;
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
			LN_PROFILE_FUNCTION();
			this->size = size;
			utils::vulkanAllocator::createBuffer(&vkVertexBuffer, size, VK_BUFFER_USAGE_VERTEX_BUFFER_BIT, VMA_MEMORY_USAGE_CPU_TO_GPU, VMA_ALLOCATION_CREATE_MAPPED_BIT);
			this->data = utils::vulkanAllocator::getAllocationInfo((uint64_t)vkVertexBuffer).pMappedData;
			memcpy_s(this->data,this->size, data, size);
		}

	}
}

