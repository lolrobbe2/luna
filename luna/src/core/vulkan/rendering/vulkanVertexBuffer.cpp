#include "vulkanVertexBuffer.h"
#include <core/vulkan/utils/vulkanAllocator.h>
#include <core/vulkan/rendering/vulkanPipeline.h>
namespace luna
{
	namespace vulkan
	{
		vulkanVertexBuffer::vulkanVertexBuffer(uint32_t size)
		{
			this->size = size;
			utils::vulkanAllocator::createBuffer(&VkVertexBuffer, size, VK_BUFFER_USAGE_VERTEX_BUFFER_BIT, VMA_MEMORY_USAGE_CPU_TO_GPU);

		}
		vulkanVertexBuffer::vulkanVertexBuffer(const void* data, uint32_t size)
		{
			setData(data, size);
		}
		vulkanVertexBuffer::~vulkanVertexBuffer()
		{
			utils::vulkanAllocator::destroyBuffer(VkVertexBuffer);
		}
		void vulkanVertexBuffer::bind(ref<renderer::pipeline> pipeline) const
		{	
			ref<vulkan::vulkanPipeline> vulkanPipeline = std::dynamic_pointer_cast<vulkan::vulkanPipeline>(pipeline);
			vulkanPipeline->bindVertexBuffer(VkVertexBuffer);
		}
		void vulkanVertexBuffer::unbind(ref<renderer::pipeline> pipeline) const
		{
			ref<vulkan::vulkanPipeline> vulkanPipeline = std::dynamic_pointer_cast<vulkan::vulkanPipeline>(pipeline);
			vulkanPipeline->unbindVertexBuffer(VkVertexBuffer);
		}
		void vulkanVertexBuffer::setData(const void* data, uint32_t size)
		{
			this->size = size;
			utils::vulkanAllocator::createBuffer(&VkVertexBuffer, size, VK_BUFFER_USAGE_VERTEX_BUFFER_BIT, VMA_MEMORY_USAGE_CPU_TO_GPU);
			this->data = utils::vulkanAllocator::getAllocationInfo((uint64_t)VkVertexBuffer).pMappedData;
			memcpy(this->data, data, size);
		}
		template<typename mesh>
		mesh& vulkanVertexBuffer::getIndex(uint32_t index)
		{
			mesh* meshData = (mesh*)data;
			return meshData[index];
		}
	}
}

