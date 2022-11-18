#pragma once
#include <core/rendering/buffer.h>
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
			virtual void bind(ref<renderer::pipeline> pipeline) const override;
			virtual void unbind(ref<renderer::pipeline> pipeline) const override;

			virtual void setData(const void* data, uint32_t size) override;
			VkBuffer getBufferHandle() { return VkVertexBuffer; };
			template<typename mesh>
			mesh& getIndex(uint32_t index);
			friend class vertexBuffer;
		private:
			VkBuffer VkVertexBuffer;
		};
	}
}


