#pragma once
#include <core/core.h>
#include <core/rendering/buffer.h>
namespace luna
{
	namespace renderer
	{
		class vertexArray
		{
		public:
			virtual ~vertexArray(){};

			virtual void bind() const = 0;
			virtual void unbind() const = 0;

			virtual void addVertexBuffer(const ref<vertexBuffer>& VertexBuffer) = 0;
			virtual void setIndexBuffer(const ref<indexBuffer>& IndexBuffer) = 0;

			virtual const std::vector<ref<vertexBuffer>>& getVertexBuffers() const = 0;
			virtual const ref<indexBuffer>& getIndexBuffer() const = 0;

			static ref<vertexArray> create();
		protected:
			std::vector<ref<vertexBuffer>> vertexBuffers;
			ref<indexBuffer> indexBuffer;
		};
	}
}
