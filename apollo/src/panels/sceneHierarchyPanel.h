#pragma once
#include <core/scene/scene.h>
namespace luna
{
	class sceneHierarchyPanel
	{

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
	private:

		uint64_t addIndent = 3;
		scene* m_Context;
		Node m_SelectionContext;
	};
}

