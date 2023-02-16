#include "texture.h"
#include <core/vulkan/rendering/vulkanTexture.h>

namespace luna
{
    namespace renderer
    {
		ref<texture> texture::create(const std::string& filePath)
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
				return ref<texture>(new vulkan::vulkanTexture(filePath));
			default:
				return ref<texture>();
			}
			return ref<texture>();
		}
	}
}

