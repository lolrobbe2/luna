#include "vulkanVertexArray.h"
namespace luna
{
	namespace vulkan
	{
		void vulkanVertexArray::bind() const
		{
			//for (const ref<renderer::vertexBuffer>& vertexBuffer : vertexBuffers) vertexBuffer->bind(pipeline);
		}
		void vulkanVertexArray::unbind() const
		{
			//for (const ref<renderer::vertexBuffer>& vertexBuffer : vertexBuffers) vertexBuffer->unbind(pipeline);
		}
		void vulkanVertexArray::addVertexBuffer(const ref<renderer::vertexBuffer>& vertexBuffer)
		{
			vertexBuffers.push_back(vertexBuffer);
		}
		void vulkanVertexArray::setIndexBuffer(const ref<renderer::indexBuffer>& indexBuffer)
		{
			this->indexBuffer = indexBuffer;
		}
	}
}
