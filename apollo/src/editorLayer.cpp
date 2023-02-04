#include "editorLayer.h"


namespace luna
{
	editorLayer::editorLayer(const std::string& name)
	{
	}
	void editorLayer::onAttach()
	{
		LN_CORE_INFO("added editor!");
	}
	void editorLayer::onDetach()
	{
	}
	void editorLayer::onUpdate(utils::timestep ts)
	{
	}
	void editorLayer::onImGuiRender()
	{
		if (ImGui::Begin("sceneHierachy"));
		ImGui::End();
	}
	void editorLayer::onEvent(Event& event)
	{
	}
}
