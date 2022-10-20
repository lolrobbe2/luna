
#include <core/rendering/renderer.h>
#include <core/vulkan/device/vulkanDevice.h>
#include <core/vulkan/rendering/vulkanPipeline.h>
#include <core/utils/shaderLibrary.h>
#include <imgui_demo.cpp>
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
				renderer::gui = ref<gui::vulkanImgui>(new gui::vulkanImgui(renderer::rendererPipeline));
				break;
			default:
				break;		
			}
		}
		void renderer::createFrame()
		{

		}
		void renderer::newFrame()
		{
			rendererPipeline->begin();
			ImGuiViewport* viewport = ImGui::GetMainViewport();
			ImVec2 windowSize = ImGui::GetContentRegionAvail();
			ImGui::DockSpaceOverViewport(viewport, ImGuiDockNodeFlags_None | ImGuiDockNodeFlags_PassthruCentralNode| ImGuiDockNodeFlags_NoResize);
			if (ImGui::Begin("settings",nullptr))
			{
				ImGui::Text(("framerate = " + std::to_string(ImGui::GetIO().Framerate) + " FPS").c_str());
				ImGui::Text(("frameTime = " + std::to_string(ImGui::GetIO().DeltaTime * 1000) + " ms").c_str());
			}
			ImGui::End();

			rendererPipeline->end();
			rendererPipeline->createCommands();
			rendererPipeline->flush();
		}

	}
}
