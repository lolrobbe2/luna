#include "editorLayer.h"
#include <core/rendering/renderer2D.h>
#include <core/input.h>
#include <core/platform/platformUtils.h>
#include <core/scene/sceneSerializer.h>
#include <core/application.h>

#include <project/visualStudio/projectGeneratorVS.h>
#include <project/projectManager.h>
#include <project/projectSerializer.h>

namespace luna
{
	static bool CreateNewProject = false;

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
					CreateNewProject = true;
				}
				if (ImGui::MenuItem("open", "Ctrl+Shift+O"))
				{
					openProject();
				}
				if (ImGui::MenuItem("close project", "Ctrl+Alt+L"))
				{
					
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
			ImGui::EndMainMenuBar();
		}
		if(CreateNewProject) ImGui::OpenPopup("CreateNewProject");
		createProjectPopup();
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

				} else {
					open();
				}
			}
			break;
		case input::N:
			if(controlPressed)
			{
				CreateNewProject = true;
			}
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
		
	}
	void editorLayer::createProjectPopup()
	{
		static std::string projectName;
		static std::filesystem::path projectDir;
		ImGuiViewport* viewport = ImGui::GetMainViewport();

		if (ImGui::BeginPopupModal("CreateNewProject", NULL, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoDocking | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoTitleBar))
		{
			ImGui::Text("Projectname:");
			inputText("##projectNameInput", projectName, viewport->WorkSize.x * 0.35f);
			ImGui::SameLine();
			if (ImGui::Button("Create Folder", ImVec2(-1.0f, 0.0f)))
			{
				std::string folderName = std::filesystem::path(projectDir).filename().string();
				if (std::filesystem::exists(projectDir) && folderName != projectName)
				{
					std::string tempProjectDir = projectDir.string();
					tempProjectDir.pop_back();
					tempProjectDir += "\\";
					tempProjectDir += projectName;
					tempProjectDir += "\0";
					projectDir = tempProjectDir;
					std::filesystem::create_directories(projectDir);

				}
			}
			if (std::filesystem::exists(std::filesystem::absolute(projectDir)) && !std::filesystem::is_empty(std::filesystem::absolute(projectDir)))
			{
				ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(0.933f, 0.824f, 0.008f, 1.0f));

				std::string warningText = "current directory is not empty, it is highly recommended to select an empty folder!";

				auto windowWidth = ImGui::GetWindowSize().x;
				auto textWidth = ImGui::CalcTextSize(warningText.c_str()).x;

				ImGui::SetCursorPosX((windowWidth - textWidth) * 0.5f);
				ImGui::Text(warningText.c_str());
				ImGui::PopStyleColor();
			}
			ImGui::Text("Project directory");
			std::string temp = projectDir.string();
			inputText("##projectDir", temp, viewport->WorkSize.x * 0.35f);
			projectDir = temp;
			ImGui::SameLine();
			if (ImGui::Button("Browse", ImVec2(-1.0f, 0.0f)))
			{
				projectDir = platform::os::openFolderDialog();
			}
			ImGui::SetCursorPosY(viewport->WorkSize.y * 0.5f * 0.9f);
			ImGui::SetCursorPosX(ImGui::GetCursorPosX() + viewport->WorkSize.x * 0.5f * 0.15f);
			if (ImGui::Button("Create and Edit ", ImVec2(viewport->WorkSize.x * 0.5f * 0.3f, 0.0f)))
			{
				project::projectGeneratorVS::generateProject(projectName, projectDir);

				project::projectConfig config;
				config.projectDirectory = projectDir;
				config.name = projectName;
				config.assetDirectory = "assets";
				config.scriptModulePath = "bin";
				config.startScene = "undefined";

				std::filesystem::create_directories(config.assetDirectory);

				ref<project::project> newProject = project::projectManager::createProject(config);
				project::projectSerializer::serialize(newProject);

				project::projectManager::setActive(newProject);

				std::filesystem::current_path(config.projectDirectory); //set currentworking dirrectory to project directory so that the relative directory's work.

				CreateNewProject = false;
				ImGui::CloseCurrentPopup();
			}
			ImGui::SameLine(viewport->WorkSize.x * 0.25f);
			if (ImGui::Button("Cancel", ImVec2(viewport->WorkSize.x * 0.5f * 0.3f, 0.0f)))
			{
				CreateNewProject = false;
				ImGui::CloseCurrentPopup();
			}
			ImGui::EndPopup();
		}
	}
	void editorLayer::openProject()
	{
		LN_CORE_INFO("openProject");
	}

	void editorLayer::inputText(const std::string& name, std::string& stringBuffer, float width, const std::string& hint)
	{
		if (width) ImGui::PushItemWidth(width);
		char buffer[256];
		memset(buffer, 0, sizeof(buffer));
		strcpy_s(buffer, stringBuffer.c_str());
		if (hint == "" && ImGui::InputText(name.c_str(), buffer, sizeof(buffer))) stringBuffer = std::string(buffer);
		else if (hint != "" && ImGui::InputTextWithHint(name.c_str(), hint.c_str(), buffer, sizeof(buffer))) stringBuffer = std::string(buffer);
		if (width) ImGui::PopItemWidth();
	}
}
