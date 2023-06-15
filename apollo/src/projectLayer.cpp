#include "projectLayer.h"
namespace luna 
{
	static std::string filter;
	static int currentItem = 0;
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
		
		ImGui::PushStyleColor(ImGuiCol_WindowBg, ImVec4(0.722f, 0.443f, 0.0f, 1.0f));
		ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(1, 0.678, 0.149, 1.0f));
		ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0.871f, 0.588f, 0.137f, 1.0f));
		ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0.8f, 0.537f, 0.11f, 1.0f));
		if(ImGui::Begin("startup",nullptr, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoDocking))
		{
			const float width = ImGui::GetWindowWidth();
			const float combo_width = width * 0.35f;
			const float inputTextWidth = width * 0.35f;
			
			inputText("##Filter Projects",filter,inputTextWidth, "Filter Projects");

			ImGui::SameLine();
			ImGui::PushItemWidth(width * 0.1f);
			ImGui::Text("sort: ");
			ImGui::SameLine();
			const char* items[] = { "Last Edited" };
			
			
			ImGui::SetNextItemWidth(combo_width);
			if (ImGui::Combo("##Last Edited", &currentItem, items, 1));
			{

			}	
			ImGui::SameLine();
		
		
			if(ImGui::Button("New Project",ImVec2(-1.0f,0.0f)));
			{
			}
			ImGui::PushStyleColor(ImGuiCol_ChildBg, ImVec4(0.839f, 0.518f, 0.0f, 1.0f));
			//ImGui::SetNextWindowPos(ImVec2(viewport->WorkPos.x + viewport->WorkSize.y * 0.05f, viewport->WorkPos.y + viewport->WorkSize.y * 0.05f));

			//ImGui::SetNextWindowViewport(viewport->ID);
			if (ImGui::BeginChild("project view", ImVec2(viewport->WorkSize.x * 0.75, viewport->WorkSize.y * 0.94f), ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoDocking))
			{
			}
			ImGui::EndChild();
			ImGui::PopStyleColor();
		}
		ImGui::End();
		ImGui::PopStyleColor(4);
	}
	void projectLayer::onEvent(Event& event)
	{
	}
	void projectLayer::inputText(const std::string& name, std::string& stringBuffer,float width = 0,const std::string& hint = "")
	{
		if (width) ImGui::PushItemWidth(width);
		char buffer[256];
		memset(buffer, 0, sizeof(buffer));
		strcpy_s(buffer, stringBuffer.c_str());
		if (hint == "" && ImGui::InputText(name.c_str(), buffer, sizeof(buffer))) stringBuffer = std::string(buffer);
		else if (ImGui::InputTextWithHint(name.c_str(), hint.c_str(), buffer, sizeof(buffer))) stringBuffer = std::string(buffer);
		if (width) ImGui::PopItemWidth();
	}
}