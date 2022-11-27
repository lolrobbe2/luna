#pragma once
#include <core/rendering/vertexArray.h>
namespace luna 
{
	namespace vulkan 
	{
		class vulkanVertexArray : public renderer::vertexArray
		{
		public:
			virtual void bind() const override;
			virtual void unbind() const override;

			virtual void addVertexBuffer(const ref<renderer::vertexBuffer>& vertexBuffer) override;
			virtual void setIndexBuffer(const ref<renderer::indexBuffer>& indexBuffer) override;
		private:
		};
	}
}


