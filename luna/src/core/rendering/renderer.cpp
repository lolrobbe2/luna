#include <core/rendering/renderer.h>
#include <core/vulkan/device/vulkanDevice.h>
#include <core/vulkan/rendering/vulkanPipeline.h>
#include <core/utils/shaderLibrary.h>
namespace luna
{
	namespace renderer
	{
		void renderer::init(const ref<vulkan::window>& window)
		{
			switch (window->graphicsApi)
			{
			case vulkan::NONE:
				LN_CORE_TRACE("NONE is not supported");
				break;
			case vulkan::OPENGL:
				LN_CORE_TRACE("OPENGL is not supported");
				__debugbreak();
				break;
			case vulkan::VULKAN:
				renderer::rendererDevice = ref<device>(new vulkan::vulkanDevice(window));
				layout.device = rendererDevice;
				layout.pipelineShaders.push_back(utils::shaderLibrary::get("fragment.glsl"));
				layout.pipelineShaders.push_back(utils::shaderLibrary::get("vertex.glsl"));
				renderer::rendererPipeline = ref<pipeline>(new vulkan::vulkanPipeline(layout));
				break;
			default:
				break;		
			}
		}
	}
}
