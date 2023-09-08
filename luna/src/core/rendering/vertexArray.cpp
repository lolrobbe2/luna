#include "vertexArray.h"
#include <core/vulkan/window/window.h>
#include <core/vulkan/rendering/vulkanVertexArray.h>
namespace luna 
{
	namespace renderer
	{
		ref<vertexArray> vertexArray::create()
		{
            switch (vulkan::window::graphicsApi)
            {
            case vulkan::NONE:
                LN_CORE_INFO(" none vertexArray not implemented");
                break;
            case vulkan::OPENGL:
                LN_CORE_INFO(" opengl veretexArray not implemented");
                break;
            case vulkan::VULKAN:
                return ref<vertexArray>(new vulkan::vulkanVertexArray());
            default:
                return ref<vertexArray>();
            }
		}
	}
}
