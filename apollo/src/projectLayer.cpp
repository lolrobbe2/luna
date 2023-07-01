#include <core/platform/platformUtils.h>
#include <project/visualStudio/projectGeneratorVS.h>
#include <project/projectManager.h>
#include <project/projectSerializer.h>
#include <core/application.h>
#include <editorLayer.h>
#include "projectLayer.h"

namespace luna 
{
	static std::string filter;
	static ref<project::project> selected;
	static int currentItem = 0;

#pragma region projectSetup
	static std::string projectName = "New Project";
	static std::filesystem::path projectDir = "";
#pragma endregion
	static bool importProjectPopup;
	static std::filesystem::path projectImportPath = "";
#pragma region projectImport
	projectLayer::projectLayer(const std::string& name)
	{
		m_DebugName = name;

		
	}
	void projectLayer::onAttach()
	{
	}
	void projectLayer::onDetach()
	{
	}
	void projectLayer::onUpdate(utils::timestep ts)
	{
	}
	void projectLayer::onImGuiRender()
	{
#ifdef IMGUI_HAS_VIEWPORT
		ImGuiViewport* viewport = ImGui::GetMainViewport();
		ImGui::SetNextWindowPos(viewport->WorkPos);
		ImGui::SetNextWindowSize(viewport->WorkSize);
		ImGui::SetNextWindowViewport(viewport->ID);
#else 
		ImGui::SetNextWindowPos(ImVec2(0.0f, 0.0f));
		ImGui::SetNextWindowSize(ImGui::GetIO().DisplaySize);
#endif
		//#ffad26
		int oldSize = ImGui::GetFont()->Scale;
		ImGui::GetFont()->Scale *= 1.5;
		ImGui::PushFont(ImGui::GetFont());
		ImGui::PushStyleColor(ImGuiCol_WindowBg, ImVec4(0.722f, 0.443f, 0.0f, 1.0f));
		ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(1, 0.678, 0.149, 1.0f));
		ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0.871f, 0.588f, 0.137f, 1.0f));
		ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0.8f, 0.537f, 0.11f, 1.0f));
		if(ImGui::Begin("startup",nullptr, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoDocking))
		{
			const float width = ImGui::GetWindowWidth();
			const float combo_width = width * 0.35f;
			const float inputTextWidth = width * 0.30f;
			
			inputText("##Filter Projects",filter,inputTextWidth, "Filter Projects");

			ImGui::SameLine();
			ImGui::Text("sort: ");
			ImGui::SameLine(width * 0.40);
			const char* items[] = { "Last Edited" };
			
			ImGui::SetNextItemWidth(combo_width);
			if (ImGui::Combo("##Last Edited", &currentItem, items, 1));
			{
				
			}	
			ImGui::SameLine();
		
		
			if(ImGui::Button("New Project",ImVec2(-1.0f,0.0f)));
			{
				if (ImGui::IsItemHovered() && ImGui::IsItemClicked()) {
					ImGui::OpenPopup("CreateNewProject");
					projectName = "New Project";
				}
			}

			ImGui::SetNextWindowPos(ImVec2(viewport->WorkPos.x + viewport->WorkSize.x * 0.25f, viewport->WorkPos.y + viewport->WorkSize.y * 0.25f));
			ImGui::SetNextWindowSize(ImVec2(viewport->WorkSize.x * 0.5f, viewport->WorkSize.y * 0.5f));

			if (ImGui::BeginPopupModal("CreateNewProject", nullptr, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoDocking | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoTitleBar))
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
					ImGui::GetFont()->Scale = oldSize;
					ImGui::PushFont(ImGui::GetFont());
					ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(0.933f, 0.824f, 0.008f, 1.0f));

					std::string warningText = "current directory is not empty, it is highly recommended to select an empty folder!";

					auto windowWidth = ImGui::GetWindowSize().x;
					auto textWidth = ImGui::CalcTextSize(warningText.c_str()).x;

					ImGui::SetCursorPosX((windowWidth - textWidth) * 0.5f);
					ImGui::Text(warningText.c_str());
					ImGui::PopStyleColor();
					ImGui::GetFont()->Scale *= 1.5f;
					ImGui::PopFont();
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

					application::application::get().pushLayer(new editorLayer(this));
				}
				ImGui::SameLine(viewport->WorkSize.x * 0.25f);
				if (ImGui::Button("Cancel", ImVec2(viewport->WorkSize.x * 0.5f * 0.3f, 0.0f)))
				{
				}
				if (ImGui::IsItemClicked()) {
					ImGui::CloseCurrentPopup();
				}
				ImGui::EndPopup();
			}

			ImGui::PushStyleColor(ImGuiCol_ChildBg, ImVec4(0.839f, 0.518f, 0.0f, 1.0f));
			//ImGui::SetNextWindowPos(ImVec2(viewport->WorkPos.x + viewport->WorkSize.y * 0.05f, viewport->WorkPos.y + viewport->WorkSize.y * 0.05f));

			//ImGui::SetNextWindowViewport(viewport->ID);
			if (ImGui::BeginChild("project view", ImVec2(viewport->WorkSize.x * 0.745, viewport->WorkSize.y * 0.94f), ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoDocking))
			{
				ImGui::PushStyleVar(ImGuiStyleVar_ButtonTextAlign, ImVec2(0.03f, 0.5f));
				for(ref<project::project> project : project::projectManager::getProjects())
				{
					std::string description = "name: ";
					description += project->getConfig().name;
					description += "\nlocation: ";
					description += project->getConfig().projectDirectory.string();
					static bool selectionChanged;
					if(selected && selected->getConfig().name == project->getConfig().name)
					{
						ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.8f, 0.537f, 0.11f, 1.0f));
						if (ImGui::Button(description.c_str(), ImVec2(-1.0f, viewport->WorkSize.y * 0.07f)))
						{

							project::projectManager::setActive(project);

							std::filesystem::current_path(selected->getConfig().projectDirectory); //set currentworking dirrectory to project directory so that the relative directory's work.

							application::application::get().pushLayer(new editorLayer(this));
						}
						ImGui::PopStyleColor();
					} else {
						if (ImGui::Button(description.c_str(), ImVec2(-1.0f, viewport->WorkSize.y * 0.07f)))
						{
							selected = project;
						}
					}
				}
				ImGui::PopStyleVar();
			}
			ImGui::EndChild();
			//ImGui::SetCursorPosX(viewport->WorkSize.x * 0.75);
			ImGui::SameLine();
			ImGui::BeginGroup();
			if (ImGui::Button("Import", ImVec2(-1.0f, 0.0f)));
			{
				if (ImGui::IsItemHovered() && ImGui::IsItemClicked()) {
					ImGui::OpenPopup("ImportProject");
				}
			}


			ImGui::SetNextWindowPos(ImVec2(viewport->WorkPos.x + viewport->WorkSize.x * 0.25f, viewport->WorkPos.y + viewport->WorkSize.y * 0.25f));
			ImGui::SetNextWindowSize(ImVec2(viewport->WorkSize.x * 0.5f, viewport->WorkSize.y * 0.2f));

			if (ImGui::BeginPopupModal("ImportProject", nullptr, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoDocking | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoTitleBar))
			{
				ImGui::Text("Project Path:");
				std::string temp = projectImportPath.string();
				inputText("##importPath", temp, viewport->WorkSize.x * 0.5f * 0.8f);
				projectImportPath = temp;
				ImGui::SameLine();
				if (ImGui::Button("Browse", ImVec2(-1.0f, 0.0f)))
				{
					projectImportPath = platform::os::openFileDialog("luna project\0*.lprj\0");
				}

				ImGui::SetCursorPosY(viewport->WorkSize.y * 0.5f * 0.3f);
				ImGui::SetCursorPosX(ImGui::GetCursorPosX() + viewport->WorkSize.x * 0.5f * 0.15f);
				if (ImGui::Button("Import and Edit ", ImVec2(viewport->WorkSize.x * 0.5f * 0.3f, 0.0f)))
				{
					if (std::filesystem::exists(projectImportPath) && projectImportPath.extension() == ".lprj")
					{
						projectDir = projectImportPath.parent_path();
						ref<project::project> importProject = project::projectSerializer::deSerialize(projectImportPath);
						importProject->getConfig().projectDirectory = projectDir;
						project::projectGeneratorVS::generateProject(projectName, projectDir);

						project::projectSerializer::serialize(importProject);

						project::projectManager::setActive(importProject);

						project::projectManager::createProject(importProject->getConfig());
						std::filesystem::current_path(projectDir); //set currentworking dirrectory to project directory so that the relative directory's work.

						application::application::get().pushLayer(new editorLayer(this));
					}
				}
				ImGui::SameLine(viewport->WorkSize.x * 0.25f);
				if (ImGui::Button("Cancel", ImVec2(viewport->WorkSize.x * 0.5f * 0.3f, 0.0f)))
				{

				}
				if (ImGui::IsItemClicked()) {
					importProjectPopup = false;
					ImGui::CloseCurrentPopup();
				}
				ImGui::EndPopup();
			}

			if (ImGui::Button("Scan", ImVec2(-1.0f, 0.0f)));
			{
			}

			ImGui::Separator();
			if (ImGui::Button("Edit", ImVec2(-1.0f, 0.0f)));
			{
				if (ImGui::IsItemClicked() && selected) {
					project::projectManager::setActive(selected);
					std::filesystem::current_path(selected->getConfig().projectDirectory); //set currentworking dirrectory to project directory so that the relative directory's work.

					application::application::get().pushLayer(new editorLayer(this));
				}
			}
			if (ImGui::Button("Run", ImVec2(-1.0f, 0.0f)));
			{
			}
			if (ImGui::Button("Rename", ImVec2(-1.0f, 0.0f)));
			{
			}
			if (ImGui::Button("Remove", ImVec2(-1.0f, 0.0f)));
			{
				if(ImGui::IsItemClicked() && selected != nullptr)
				{
					project::projectManager::removeProject(selected);
				}
			}
			if (ImGui::Button("Remove Missing", ImVec2(-1.0f, 0.0f)));
			{
			}
			ImGui::EndGroup();
			ImGui::PopStyleColor();
		}
		ImGui::End();

		ImGui::PopStyleColor(4);
		ImGui::GetFont()->Scale = oldSize;
		ImGui::PopFont();
	}
	void projectLayer::onEvent(Event& event)
	{
	}
	void projectLayer::inputText(const std::string& name, std::string& stringBuffer,float width,const std::string& hint)
	{
		if (width) ImGui::PushItemWidth(width);
		char buffer[256];
		memset(buffer, 0, sizeof(buffer));
		strcpy_s(buffer, stringBuffer.c_str());
		if (hint == "" && ImGui::InputText(name.c_str(), buffer, sizeof(buffer))) stringBuffer = std::string(buffer);
		else if (hint!= "" && ImGui::InputTextWithHint(name.c_str(), hint.c_str(), buffer, sizeof(buffer))) stringBuffer = std::string(buffer);
		if (width) ImGui::PopItemWidth();
	}
}