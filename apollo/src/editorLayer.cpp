#include "editorLayer.h"
#include <core/rendering/renderer2D.h>
#include <core/input.h>
#include <core/platform/platformUtils.h>
#include <core/scene/sceneSerializer.h>

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
		activeScene->onUpdate(ts);
		activeScene->onUpdateEditor(ts);
	}
	void editorLayer::onImGuiRender()
	{
		
		if(ImGui::BeginMainMenuBar())
		{
			if (ImGui::BeginMenu("file")) 
			{
				if (ImGui::MenuItem("save as", "Ctrl+Shift+S"))
				{
					saveAs();
				}
				if (ImGui::MenuItem("open", "Ctrl+O"))
				{
					open();

				}
				ImGui::EndMenu();
			}

		}
		
		ImGui::EndMainMenuBar();
		scenePanel->onImGuiRender();

	}
	void editorLayer::onEvent(Event& Event)
	{
	
		activeScene->onEvent(Event);
		eventDispatcher dispatcher(Event);
		dispatcher.dispatch<keyPressedEvent>(LN_BIND_EVENT_FN(editorLayer::onKeyPressedEvent));
	}
	bool editorLayer::onKeyPressedEvent(keyPressedEvent& Event)
	{
		if (Event.isRepeat()) return false;

		bool controlPressed = input::input::isKeyPressed(input::LeftControl) || input::input::isKeyPressed(input::RightControl);
		bool shiftPressed = input::input::isKeyPressed(input::LeftShift) || input::input::isKeyPressed(input::RightShift);

		switch (Event.getkeyCode())
		{
		case input::S:
			if(controlPressed && shiftPressed)
			{
				saveAs();
			}
			break;
		case input::O:
			if (controlPressed)
			{
				open();
			}
			break;
		default:
			break;
		}
	}
	void editorLayer::saveAs()
	{
		std::string filePath = platform::os::saveFileDialog("luna scene\0*.lscn\0");
		if (!filePath.size()) return;
		sceneSerializer::serialize(activeScene.get(), filePath);
	}
	void editorLayer::open()
	{
		std::string filePath = platform::os::openFilaDialog("luna scene\0*.lscn\0");
		if (!filePath.size()) return;
		scene* nonActiveScene = sceneSerializer::deSerialize(filePath);
		if (nonActiveScene)
		{
			activeScene = ref<scene>(nonActiveScene);
			scenePanel->setContext(activeScene);
		}
	}
}
