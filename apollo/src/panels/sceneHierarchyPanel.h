#pragma once
#include <core/scene/scene.h>
namespace luna
{
	class sceneHierarchyPanel
	{
	public:


	public:
		sceneHierarchyPanel() = default;
		sceneHierarchyPanel(const ref<scene>& scene);
		void setContext(const ref<scene>& scene);

		void onImGuiRender();

		Node getSelectedNode() const { return m_SelectionContext; }
		void setSelectedNode(Node Node);
	private:
		template<typename T>
		void displayAddComponentEntry(const std::string& entryName);

		void drawEntityNode(Node Node,uint32_t indent);
		void drawComponents(Node Node);
		void drawNodeSelectionList();

		bool addNodeSelection(const std::string& nodeName,objectDB::classInfo* classInfo);

		void inputText(const std::string& name, std::string& stringBuffer);
	private:

		uint64_t addIndent = 5;
		scene* m_Context;
		Node m_SelectionContext;
		Node m_Selected;
		std::string m_ListSelected = "";
	};
	template<typename T>
	inline void sceneHierarchyPanel::displayAddComponentEntry(const std::string& entryName)
	{
	}
}

