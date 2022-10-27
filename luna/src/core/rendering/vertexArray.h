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
			virtual ~vertexArray();

			virtual void Bind() const = 0;
			virtual void Unbind() const = 0;

			virtual void AddVertexBuffer(const ref<vertexBuffer>& vertexBuffer) = 0;
			virtual void SetIndexBuffer(const ref<indexBuffer>& indexBuffer) = 0;
			
		private:
			std::vector<ref<vertexBuffer>> vertexBuffers;
			ref<indexBuffer>vertexBuffers;
		};
	}
}
