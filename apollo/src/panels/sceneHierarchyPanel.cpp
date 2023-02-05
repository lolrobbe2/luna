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
				if(!Node.hasComponent<parentComponent>()) drawEntityNode(Node,0);
			});
			/*
			if (ImGui::IsMouseDown(0) && ImGui::IsWindowHovered())
				m_SelectionContext = {};
			*/


		}
		ImGui::End();
	}

	void sceneHierarchyPanel::setSelectedNode(Node Node)
	{
		m_SelectionContext = Node;
	}

	void sceneHierarchyPanel::drawEntityNode(Node Node, uint32_t indent)
	{
		
		std::string buttonText = Node.getName();
		bool isOpen = ImGui::TreeNodeEx((void*)Node.getUUID().getId(), ImGuiTreeNodeFlags_OpenOnArrow, buttonText.c_str());
	
		
		if (ImGui::BeginDragDropTarget())
		{
			const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("node");
			if (payload != nullptr && payload->DataSize == 0)
			{
				Node.addChild(m_SelectionContext);
				
			}
			ImGui::EndDragDropTarget();
		}

		if (ImGui::BeginDragDropSource()) {
			ImGui::SetDragDropPayload("node", nullptr, 0);
			m_SelectionContext = Node;
			ImGui::Text(Node.getName().c_str());
			ImGui::EndDragDropSource();
		}

		if (isOpen)
		{
			if (Node.hasComponent<childComponent>())
			{
				ImGui::Indent(indent + addIndent);
				auto& childs = Node.getComponent<childComponent>().childs;

				for (auto child : childs)
				{
					luna::Node _Node{ child,m_Context };
					if (_Node.getComponent<parentComponent>().parentId == Node.getComponent<idComponent>().id)
					{
						drawEntityNode(_Node, indent + addIndent);
					}
				}
				ImGui::Unindent(indent + addIndent);
			}

			ImGui::TreePop();
		}
		if (ImGui::IsItemClicked() && !ImGui::IsItemToggledOpen())
		{
			LN_CORE_INFO("clicked");
		}
	}
}