#include "editorLayer.h"
#include <core/rendering/renderer2D.h>
#include <core/input.h>
#include <core/platform/platformUtils.h>
#include <core/scene/sceneSerializer.h>
#include <core/application.h>

namespace luna
{
	
	editorLayer::editorLayer(layer* prjLayer,const std::string& name)
	{
		this->prjLayer = prjLayer;
		m_DebugName = name;
		activeScene = ref<scene>(new scene());
		scenePanel = ref<sceneHierarchyPanel>(new sceneHierarchyPanel());
	}
	void editorLayer::onAttach()
	{
		if (prjLayer) application::application::get().popLayer(prjLayer);
		LN_CORE_INFO("added editor!");
		scenePanel->setContext(activeScene);
		utils::scriptUtils::setContext(activeScene.get());
	}
	void editorLayer::onDetach()
	{
		activeScene.~shared_ptr();
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
			if (ImGui::BeginMenu("project"))
			{
				if(ImGui::MenuItem("create new","Ctrl+N"))
				{
					createProject();
				}
				if (ImGui::MenuItem("open", "Ctrl+Shift+O"))
				{
					openProject();
				}
				ImGui::EndMenu();
			}
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

		ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0, 0));

		if (ImGui::Begin("scene"));
		{
			ImVec2 scrollPos = ImGui::GetCursorScreenPos();
			ImVec2 viewportPanelSize = ImGui::GetContentRegionAvail();
			ImVec2 mousePos = ImGui::GetMousePos();
			glm::vec2 windowMousePos;
			windowMousePos.x = mousePos.x - scrollPos.x;
			windowMousePos.y = mousePos.y - scrollPos.y;
			renderer::renderer::setSceneMouse(windowMousePos);
			renderer::renderer::setSceneDimensions({ viewportPanelSize.x, viewportPanelSize.y });

			std::string text = (activeScene->m_IsRunning) ? "stop" : "play";
			ImVec2 buttonSize = { ImGui::GetContentRegionAvail().x + 1.0f,30.0f };
			ImGui::Image(renderer::renderer::getWindowImage(), ImGui::GetContentRegionAvail());
			ImGui::SameLine(0.000001f);
			if(ImGui::Button(text.c_str(), buttonSize));
			{
				if (ImGui::IsItemClicked())
				{
					activeScene->m_IsRunning = !activeScene->m_IsRunning;
					if (activeScene->m_IsRunning) activeScene->onPlayScene();
					else activeScene->onStopScene();
				}
			}
			
		}

		ImGui::PopStyleVar(1);
		ImGui::End();
		
		scenePanel->onImGuiRender();

	}
	void editorLayer::onEvent(Event& Event)
	{
		eventDispatcher dispatcher(Event);
		dispatcher.dispatch<keyPressedEvent>(LN_BIND_EVENT_FN(editorLayer::onKeyPressedEvent));
		activeScene->onEvent(Event);
	
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
				if (shiftPressed) 
				{
					openProject();
				} else {
					open();
				}
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
		std::string filePath = platform::os::openFileDialog("luna scene\0*.lscn\0");
		if (!filePath.size()) return;

		scenePanel = nullptr;
		scenePanel = ref<sceneHierarchyPanel>(new sceneHierarchyPanel());
		//activeScene = nullptr;		
		activeScene = ref<scene>(sceneSerializer::deSerialize(filePath));
		scenePanel->setContext(activeScene);
		scenePanel->setSelectedNode(Node());
		
	}
	void editorLayer::createProject()
	{
		LN_CORE_INFO("createProject");
	}
	void editorLayer::openProject()
	{
		LN_CORE_INFO("openProject");
	}
}
