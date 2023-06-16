#include "projectLayer.h"
namespace luna 
{
	static std::string filter;
	static int currentItem = 0;

#pragma region projectSetup
	static bool newProjectPopup;
	static std::string projectName = "New Project";
	static std::string projectDir = "";
#pragma endregion
	static bool newProjectPopup;
	static std::string projectImportPath = "";
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
					newProjectPopup = !newProjectPopup;
				}
			}
			ImGui::PushStyleColor(ImGuiCol_ChildBg, ImVec4(0.839f, 0.518f, 0.0f, 1.0f));
			//ImGui::SetNextWindowPos(ImVec2(viewport->WorkPos.x + viewport->WorkSize.y * 0.05f, viewport->WorkPos.y + viewport->WorkSize.y * 0.05f));

			//ImGui::SetNextWindowViewport(viewport->ID);
			if (ImGui::BeginChild("project view", ImVec2(viewport->WorkSize.x * 0.745, viewport->WorkSize.y * 0.94f), ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoDocking))
			{
			}
			ImGui::EndChild();
			//ImGui::SetCursorPosX(viewport->WorkSize.x * 0.75);
			ImGui::SameLine();
			ImGui::BeginGroup();
			if (ImGui::Button("Import", ImVec2(-1.0f, 0.0f)));
			{
			}
			if (ImGui::Button("Scan", ImVec2(-1.0f, 0.0f)));
			{
			}

			ImGui::Separator();
			if (ImGui::Button("Edit", ImVec2(-1.0f, 0.0f)));
			{
			}
			if (ImGui::Button("Run", ImVec2(-1.0f, 0.0f)));
			{
			}
			if (ImGui::Button("Rename", ImVec2(-1.0f, 0.0f)));
			{
			}
			if (ImGui::Button("Remove", ImVec2(-1.0f, 0.0f)));
			{
			}
			if (ImGui::Button("Remove Missing", ImVec2(-1.0f, 0.0f)));
			{
			}
			ImGui::EndGroup();
			ImGui::PopStyleColor();
		}
		ImGui::End();
		if (newProjectPopup) ImGui::OpenPopup("CreateNewProject");	

		ImGui::SetNextWindowPos(ImVec2(viewport->WorkPos.x + viewport->WorkSize.x * 0.25f, viewport->WorkPos.y + viewport->WorkSize.y * 0.25f));
		ImGui::SetNextWindowSize(ImVec2(viewport->WorkSize.x * 0.5f, viewport->WorkSize.y * 0.5f));
		
		if (ImGui::BeginPopup("CreateNewProject", ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoDocking | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoScrollbar))
		{
			projectName = "New Project";
			ImGui::Text("Projectname:");
			inputText("##projectNameInput", projectName, viewport->WorkSize.x * 0.35f);
			ImGui::SameLine();
			if (ImGui::Button("Create Folder", ImVec2(-1.0f, 0.0f)))
			{

			}
			ImGui::Text("Project directory");
			inputText("##projectDir", projectDir, viewport->WorkSize.x * 0.35f);
			ImGui::SameLine();
			if (ImGui::Button("Browse", ImVec2(-1.0f, 0.0f))) 
			{

			}
			ImGui::SetCursorPosY(viewport->WorkSize.y * 0.5f * 0.9f);
			ImGui::SetCursorPosX(ImGui::GetCursorPosX() + viewport->WorkSize.x * 0.5f * 0.15f);
			if (ImGui::Button("Create and Edit ", ImVec2(viewport->WorkSize.x * 0.5f * 0.3f, 0.0f)))
			{

			}
			ImGui::SameLine(viewport->WorkSize.x * 0.25f );
			if (ImGui::Button("Cancel", ImVec2(viewport->WorkSize.x * 0.5f * 0.3f, 0.0f)))
			{

			}
			if (ImGui::IsItemClicked()) {
				newProjectPopup = false;
				ImGui::CloseCurrentPopup();
			}
			ImGui::EndPopup();
		}

		ImGui::SetNextWindowPos(ImVec2(viewport->WorkPos.x + viewport->WorkSize.x * 0.25f, viewport->WorkPos.y + viewport->WorkSize.y * 0.25f));
		ImGui::SetNextWindowSize(ImVec2(viewport->WorkSize.x * 0.5f, viewport->WorkSize.y * 0.2f));

		if (ImGui::BeginPopup("ImportProject", ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoDocking | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoScrollbar))
		{

		}

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