
#include <core/rendering/renderer.h>
//vulkan
#include <core/vulkan/device/vulkanDevice.h>
#include <core/vulkan/rendering/vulkanPipeline.h>
#include <core/vulkan/rendering/vulkanVertexArray.h>
#include <core/vulkan/rendering/vulkanVertexBuffer.h>
#include <core/vulkan/rendering/vulkanIndexBuffer.h>
#include <core/utils/shaderLibrary.h>
#include <core/vulkan/utils/vulkanAllocator.h>
#include <imgui_demo.cpp>
#include "renderer.h"
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
				//layout.pipelineShaders.push_back(utils::shaderLibrary::get("fragment.glsl"));
				//layout.pipelineShaders.push_back(utils::shaderLibrary::get("vertex.glsl"));
				renderer::rendererPipeline = ref<pipeline>(new vulkan::vulkanPipeline(layout));
				renderer::gui = ref<gui::vulkanImgui>(new gui::vulkanImgui(renderer::rendererPipeline));
				break;
			default:
				break;		
			}
		}
		void renderer::shutdown()
		{
			gui.~shared_ptr();
			rendererPipeline.~shared_ptr();
			rendererDevice.~shared_ptr();
		}

		void renderer::createFrame()
		{

		}
		void renderer::newFrame()
		{
			LN_PROFILE_SCOPE("newframe");

			utils::vulkanAllocator::flush(); //TODO run as async or put in threadpool.
			rendererPipeline->end();
			
			rendererPipeline->flush();
		}

		ImTextureID renderer::getWindowImage()
		{
			if (rendererPipeline) return rendererPipeline->getWindowImage();
			return nullptr;
		}

		void renderer::beginScene()
		{
			rendererPipeline->clear();
			rendererPipeline->begin();
			
		}

		void renderer::Submit(const ref<vertexArray>& vertexArray,const uint64_t& indexCount)
		{
			const std::vector<uint64_t> texturesEmpty;
			rendererPipeline->drawIndexed(vertexArray,texturesEmpty, indexCount);
		}

		void renderer::Submit(const ref<vertexArray>& vertexArray, std::vector<uint64_t> textures, const uint64_t& indexCount)
		{
			rendererPipeline->drawIndexed(vertexArray, textures, indexCount);
		}

		void renderer::endScene()
		{
		}

	}
}


