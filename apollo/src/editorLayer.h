#pragma once
#include <core/utils/layer.h>
#include <panels/sceneHierarchyPanel.h>
#include <core/events/keyEvent.h>
namespace luna
{
	class editorLayer : public utils::layer
	{
	public:
		editorLayer(layer* prjLayer, const std::string& name = "editor layer");
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
		void openProjectPopup();
		void createProjectPopup();
		void inputText(const std::string& name, std::string& stringBuffer, float width, const std::string& hint = "");
		layer* prjLayer = nullptr;
		ref<scene> activeScene;
		ref<sceneHierarchyPanel> scenePanel;
	};
}


