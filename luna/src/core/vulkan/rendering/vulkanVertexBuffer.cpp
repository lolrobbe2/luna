#include "vulkanVertexBuffer.h"
namespace luna
{
	namespace vulkan
	{
		void vulkanVertexBuffer::bind() const 
		{
		}
		void vulkanVertexBuffer::unbind() const
		{
		}
		void vulkanVertexBuffer::setData(const void* data, uint32_t size)
		{
		}
		const renderer::bufferLayout& vulkanVertexBuffer::getLayout() const
		{
			// TODO: insert return statement here
			return bufferLayout;
		}
		void vulkanVertexBuffer::setLayout(const renderer::bufferLayout& layout)
		{
		}
	}
}

