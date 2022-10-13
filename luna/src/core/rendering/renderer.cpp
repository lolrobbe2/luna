
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
			auto dockspaceID = ImGui::GetID("HUB_DockSpace");
			ImGui::DockSpace(dockspaceID, ImVec2(0.0f, 0.0f), ImGuiDockNodeFlags_None | ImGuiDockNodeFlags_PassthruCentralNode/*|ImGuiDockNodeFlags_NoResize*/);
			if (ImGui::Begin("settings"))
			{
				ImGui::Text("test");
				
			} 
			ImGui::End();
			rendererPipeline->end();
			rendererPipeline->createCommands();
			rendererPipeline->flush();
		}

	}
}
