#include "buffer.h"
#include <core/vulkan/window/window.h>
#include <core/vulkan/rendering/vulkanVertexBuffer.h>
#include <core/vulkan/rendering/vulkanIndexBuffer.h>
#include "buffer.h"
#include "buffer.h"
namespace luna
{
	namespace renderer
	{ 
		ref<vertexBuffer> vertexBuffer::create(uint32_t size)
		{
			switch (vulkan::window::graphicsApi)
			{
			case vulkan::NONE:
				LN_CORE_ERROR("NONE is currently not supported!");
				break;
			case vulkan::OPENGL:
				LN_CORE_ERROR("OPENGL is currently not supported!");
				break;
			case vulkan::VULKAN:
				return ref<vertexBuffer>(new vulkan::vulkanVertexBuffer(size));
			default:
				return ref<vertexBuffer>();
			}
			return ref<vertexBuffer>();
		}
		ref<vertexBuffer> vertexBuffer::create(float* vertices, uint32_t size)
		{
		
			switch (vulkan::window::graphicsApi)
			{
			case vulkan::NONE:
				LN_CORE_ERROR("NONE is currently not supported!");
				break;
			case vulkan::OPENGL:
				LN_CORE_ERROR("OPENGL is currently not supported!");
				break;
			case vulkan::VULKAN:
				return ref<vertexBuffer>(new vulkan::vulkanVertexBuffer(vertices,size));
			default:
				return ref<vertexBuffer>();
			}
			return ref<vertexBuffer>();
		}
		ref<indexBuffer> indexBuffer::create(uint32_t* indices, uint32_t count)
		{
			switch (vulkan::window::graphicsApi)
			{
			case vulkan::NONE:
				LN_CORE_ERROR("NONE is currently not supported!");
				break;
			case vulkan::OPENGL:
				LN_CORE_ERROR("OPENGL is currently not supported!");
				break;
			case vulkan::VULKAN:
				return ref<indexBuffer>(new vulkan::vulkanIndexBuffer(indices, count));
			default:
				return ref<indexBuffer>();
			}
			return ref<indexBuffer>();
		}
		ref<indexBuffer> indexBuffer::create(uint32_t count)
		{
			switch (vulkan::window::graphicsApi)
			{
			case vulkan::NONE:
				LN_CORE_ERROR("NONE is currently not supported!");
				break;
			case vulkan::OPENGL:
				LN_CORE_ERROR("OPENGL is currently not supported!");
				break;
			case vulkan::VULKAN:
				return ref<indexBuffer>(new vulkan::vulkanIndexBuffer(count));
			default:
				return ref<indexBuffer>();
			}
			return ref<indexBuffer>();
		}
	}
}


