#include "shader.h"
#include <core/vulkan/window/window.h>
namespace luna
{
    namespace renderer
    {
        
        ref<shader> shader::create(const std::string& filepath)
        {
            switch (vulkan::window::graphicsApi)
            {
            case vulkan::NONE:
                LN_CORE_INFO(" none shaders not implemented");
                break;
            case vulkan::OPENGL:
                LN_CORE_INFO(" opengl shaders not implemented");
                break;
            case vulkan::VULKAN:
                LN_CORE_INFO(" vulkan shaders not implemented");
            default:
                break;
            }
            return ref<shader>();
        }
        ref<shader> shader::create(const std::vector<uint8_t> shaderSrc)
        {
            switch (vulkan::window::graphicsApi)
            {
            case vulkan::NONE:
                LN_CORE_INFO(" none shaders not implemented");
                break;
            case vulkan::OPENGL:
                LN_CORE_INFO(" opengl shaders not implemented");
                break;
            case vulkan::VULKAN:
                LN_CORE_INFO(" vulkan shaders not implemented");
            default:
                break;
            }
        }
    }
}
