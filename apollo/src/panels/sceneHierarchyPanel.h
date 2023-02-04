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

		void drawEntityNode(Node Node);
		void drawComponents(Node Node);
	private:
		scene* m_Context;
		Node m_SelectionContext;
	};
}

