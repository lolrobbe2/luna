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
		}

		ref<texture> texture::create(const uint64_t& handle, const glm::vec2& dimensions)
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
				return ref<texture>(new vulkan::vulkanTexture(handle,dimensions));
			default:
				return ref<texture>();
			}
		}

		ref<font> font::create(const std::string& filePath)
		{
			LN_CORE_WARN("this function is deprecated by the assetManger");
			switch (vulkan::window::graphicsApi)
			{
			case vulkan::NONE:
				LN_CORE_ERROR("NONE is currently not supported!");
				break;
			case vulkan::OPENGL:
				LN_CORE_ERROR("OPENGL is currently not supported!");
				break;
			case vulkan::VULKAN:
				return ref<font>(new vulkan::vulkanFont(filePath));
			default:
				return ref<font>();
			}
		}
	}
}

