#pragma once
#include <core/rendering/buffer.h>
namespace luna
{
	namespace vulkan
	{
		class vulkanVertexBuffer : public renderer::vertexBuffer
		{
		public:
			virtual void bind()const override;
			virtual void unbind() const override;

			virtual void setData(const void* data, uint32_t size) override;

			virtual const renderer::bufferLayout& getLayout() const override;
			virtual void setLayout(const renderer::bufferLayout& layout) override;
			friend class vertexBuffer;
		};
	}
}


