#pragma once
#include <core/utils/layer.h>
#include <panels/sceneHierarchyPanel.h>
#include <panels/contentBrowserPanel.h>
#include <core/events/keyEvent.h>

namespace luna
{
	class editorLayer : public utils::layer
	{
	public:
		editorLayer(const std::string& name = "editor layer");
		virtual ~editorLayer() = default;
		
		virtual void onAttach() override;
		virtual void onDetach()  override;
		virtual void onUpdate(utils::timestep ts)  override;
		virtual void onImGuiRender()  override;
		virtual void onEvent(Event& event) override;
		bool onKeyPressedEvent(keyPressedEvent& Event);
	private:
		void saveAs();
		void open();
		ref<scene> activeScene;
		ref<sceneHierarchyPanel> scenePanel;
		ref<contentBrowserPanel> contentPanel;
	};
}


