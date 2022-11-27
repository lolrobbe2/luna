#pragma once
#include <core/rendering/buffer.h>
#include <lnpch.h>
namespace luna
{
	namespace vulkan
	{
		class vulkanVertexBuffer : public renderer::vertexBuffer
		{
		public:
			vulkanVertexBuffer(uint32_t size);
			vulkanVertexBuffer(const void* data, uint32_t size);
			virtual ~vulkanVertexBuffer();
			virtual void bind() const override;
			virtual void unbind() const override;

			virtual void setData(const void* data, uint32_t size) override;
		protected:
			friend class vulkanPipeline;
			VkBuffer vkVertexBuffer;
		};
	}
}


