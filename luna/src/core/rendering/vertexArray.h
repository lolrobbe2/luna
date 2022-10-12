#pragma once
#include <core/core.h>
namespace luna
{
	namespace rebderer
	{
		class vertexArray
		{
		public:
			virtual ~vertexArray();

			virtual void Bind() const = 0;
			virtual void Unbind() const = 0;

			virtual void AddVertexBuffer(const ref<VertexBuffer>& vertexBuffer) = 0;
			virtual void SetIndexBuffer(const ref<IndexBuffer>& indexBuffer) = 0;
		private:
			
		};
	}
}
