#include "editorLayer.h"
#include <core/rendering/renderer2D.h>

namespace luna
{
	editorLayer::editorLayer(const std::string& name)
	{
		activeScene = ref<scene>(new scene());
		scenePanel = ref<sceneHierarchyPanel>(new sceneHierarchyPanel());
	}
	void editorLayer::onAttach()
	{
		LN_CORE_INFO("added editor!");
		scenePanel->setContext(activeScene);
	}
	void editorLayer::onDetach()
	{
	}
	void editorLayer::onUpdate(utils::timestep ts)
	{
		activeScene->onUpdateEditor(ts);
	}
	void editorLayer::onImGuiRender()
	{

		scenePanel->onImGuiRender();

	}
	void editorLayer::onEvent(Event& event)
	{
	}
}
