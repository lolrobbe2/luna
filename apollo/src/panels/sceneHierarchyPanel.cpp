#include "sceneHierarchyPanel.h"
namespace luna
{
	sceneHierarchyPanel::sceneHierarchyPanel(const ref<scene>& context)
	{
		setContext(context);
	}

	void sceneHierarchyPanel::setContext(const ref<scene>& context)
	{
		m_Context = context.get();
	}

	void sceneHierarchyPanel::onImGuiRender()
	{
		ImGui::Begin("Scene Hierarchy");
		if (m_Context)
		{
			if (ImGui::Button("add node", ImVec2(60, 30)))
			{
				m_Context->addNode<Node>("node");
			}
			m_Context->m_Registry.each([&](auto entityID)
			{
				Node Node{ entityID , m_Context };
				if(!Node.hasComponent<childComponent>()) drawEntityNode(Node);
			});

			if (ImGui::IsMouseDown(0) && ImGui::IsWindowHovered())
				m_SelectionContext = {};



		}
		ImGui::End();
	}

	void sceneHierarchyPanel::setSelectedNode(Node Node)
	{
		m_SelectionContext = Node;
	}

	void sceneHierarchyPanel::drawEntityNode(Node Node)
	{
		if (ImGui::TreeNode((void*)Node.getUUID().getId(), Node.getName().c_str()))
		{
			

			if(Node.hasComponent<childComponent>())
			{
				auto& childs = Node.getComponent<childComponent>().childs;
				for (auto child : childs)
				{
					luna::Node Node{ child,m_Context };
					drawEntityNode(Node);
				}
			}
			ImGui::TreePop();
		}
		if (ImGui::BeginDragDropTarget())
		{
			const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("node", ImGuiDragDropFlags_None);
			if (payload != nullptr && payload->DataSize == 0)
			{
				
				Node.addChild(m_SelectionContext);
			}
			ImGui::EndDragDropTarget();
		}

		if (ImGui::BeginDragDropSource(ImGuiDragDropFlags_None)) {
			ImGui::SetDragDropPayload("node", nullptr, 0);
			m_SelectionContext = Node;
			ImGui::Button(Node.getName().c_str(), ImVec2(50, 50));
			ImGui::EndDragDropSource();
		}
	}
}