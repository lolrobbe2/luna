#include "shader.h"
#include <core/vulkan/window/window.h>
#include <core/vulkan/rendering/vulkanShader.h>
namespace luna
{
    namespace renderer
    {
        
        ref<shader> shader::create(const std::string& filepath)
        {
            shader* shader;
            switch (vulkan::window::graphicsApi)
            {
            case vulkan::NONE:
                LN_CORE_INFO(" none shaders not implemented");
                break;
            case vulkan::OPENGL:
                LN_CORE_INFO(" opengl shaders not implemented");
                break;
            case vulkan::VULKAN:
                shader = new vulkan::vulkanShader(filepath);
                return ref<renderer::shader>(shader);
            default:
                break;
            }
            return ref<renderer::shader>();
        }
        ref<shader> shader::create(const std::vector<uint8_t>& shaderSrc)
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
               
            default:
                break;
            }
            return ref<shader>();
        }
    }
}
