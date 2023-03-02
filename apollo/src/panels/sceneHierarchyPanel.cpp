#include "sceneHierarchyPanel.h"
#include <nodes/ui/spriteNode.h>
#include <nodes/ui/labelNode.h>
#include <core/rendering/renderer2D.h>
#include <glm/gtc/type_ptr.hpp>
#include <core/platform/platformUtils.h>

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

		if (m_Context)
		{
			if (ImGui::Begin("settings"))
			{
				ImGui::Text(("framerate = " + std::to_string(ImGui::GetIO().Framerate) + " FPS").c_str());
				ImGui::Text(("frameTime = " + std::to_string(ImGui::GetIO().DeltaTime * 1000) + " ms").c_str());
				ImGui::Text(("drawCalls = " + std::to_string(renderer::renderer2D::getStats().drawCalls)).c_str());
				ImGui::Text(("quadCount = " + std::to_string(renderer::renderer2D::getStats().quadCount)).c_str());
				ImGui::Text(("vertexCount = " + std::to_string(renderer::renderer2D::getStats().getTotalVertexCount())).c_str());
				ImGui::Text(("indexCount = " + std::to_string(renderer::renderer2D::getStats().getTotalIndexCount())).c_str());
			}
			ImGui::End();


			ImGui::Begin("scene Hierarchy");
			if (ImGui::Button("add node", ImVec2(80, 30))) ImGui::OpenPopup("add node");
			
			if (ImGui::BeginPopupModal("add node",nullptr, ImGuiWindowFlags_NoMove))
			{
				ImGui::SetWindowSize(ImVec2(1000, 500));
				ImVec2 windowPos = ImGui::GetMainViewport()->GetCenter();
				ImGui::SetWindowPos({windowPos.x - ImGui::GetWindowSize().x / 2, windowPos.y - ImGui::GetWindowSize().y / 2 });
				if (ImGui::Button("exit node selection", ImVec2(200, 20)))
				{
					m_ListSelected = 0;
					ImGui::CloseCurrentPopup();
				}
				ImGui::SameLine(ImGui::GetContentRegionAvail().x-200);
				if(ImGui::Button("add selected node",ImVec2(200,20)))
				{
					//TODO implement register node type method;
					switch (m_ListSelected)
					{
					case node:
						m_Context->addNode<Node>("node");
						break;
					case spriteNode:
						m_Context->addNode<nodes::spriteNode>("spriteNode");
						break;
					case labelNode:
						m_Context->addNode<nodes::labelNode>("labelNode");
						break;
					default:
						break;
					}
					m_ListSelected = 0;
					ImGui::CloseCurrentPopup();
				}
				drawNodeSelectionList();
				ImGui::EndPopup();
			}

			m_Context->m_Registry.each([&](auto entityID)
			{
				Node Node{ entityID , m_Context };
				if(!Node.hasComponent<parentComponent>()) drawEntityNode(Node,0);
			});

			if (ImGui::IsMouseDown(0) && ImGui::IsWindowHovered())
				m_Selected = {};
			
			ImGui::End();
			//ImGui::DockBuilderDockWindow("properties", dock_id_Top_left);
			ImGui::Begin("properties");
			if(m_Selected) drawComponents(m_Selected);
			ImGui::End();
		}

	}

	void sceneHierarchyPanel::setSelectedNode(Node Node)
	{
		m_SelectionContext = Node;
	}

	void sceneHierarchyPanel::drawEntityNode(Node Node, uint32_t indent)
	{
		
		std::string buttonText = Node.getName();
		ImGuiTreeNodeFlags flags = (m_Selected == Node) ? ImGuiTreeNodeFlags_Selected : 0;
		bool isOpen = ImGui::TreeNodeEx((void*)Node.getUUID().getId(), ImGuiTreeNodeFlags_OpenOnArrow | flags, buttonText.c_str());

		
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

		if (ImGui::IsItemClicked() && !ImGui::IsItemToggledOpen()) m_Selected = Node;

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
		

	}
	void sceneHierarchyPanel::drawComponents(Node Node)
	{
		if (Node.hasComponent<idComponent>())
		{
			auto& id = Node.getComponent<idComponent>();
			std::stringstream sstream;
			sstream << std::hex << id.id;
			std::string result = sstream.str();
			ImGui::LabelText(result.c_str(), "id");
			ImGui::Separator();

		}
		if(Node.hasComponent<tagComponent>())
		{
			auto& tag = Node.getComponent<tagComponent>().tag;
			char buffer[256];
			memset(buffer, 0, sizeof(buffer));
			strcpy_s(buffer, tag.c_str());
			if (ImGui::InputText("name", buffer, sizeof(buffer)))tag = std::string(buffer);
			ImGui::Separator();
		}
		if(Node.hasComponent<transformComponent>())
		{
			auto& transform = Node.getComponent<transformComponent>();
			if (ImGui::TreeNodeEx((void*)typeid(transformComponent).hash_code(), 0, "transform"))
			{

				ImGui::DragFloat3("position", glm::value_ptr(transform.translation), 0.25f);
				ImGui::DragFloat3("rotation", glm::value_ptr(transform.rotation), 0.25f);
				ImGui::DragFloat3("scale", glm::value_ptr(transform.scale), 0.25f);
				ImGui::TreePop();
			}
			ImGui::Separator();
		}
		if (Node.hasComponent<spriteRendererComponent>())
		{
			auto& sprite = Node.getComponent<spriteRendererComponent>();
			if (ImGui::TreeNodeEx((void*)typeid(spriteRendererComponent).hash_code(), 0, "sprite"))
			{
				ImGui::DragFloat4("color", glm::value_ptr(sprite.color), 0.25f);
				char buffer[256];
				memset(buffer, 0, sizeof(buffer));
				strcpy_s(buffer, sprite.filePath.c_str());
				if (ImGui::InputText("file path", buffer, sizeof(buffer))) sprite.filePath = std::string(buffer);
				ImGui::SameLine();
				if(ImGui::Button("select image"))
				{
					sprite.filePath = luna::platform::os::openFilaDialog("image (*.png)\0*.png\0");
					sprite.texture = renderer::texture::create(sprite.filePath);
				}

				ImGui::TreePop();
			}
			ImGui::Separator();
		}
		if (Node.hasComponent<labelRendererComponent>())
		{
			auto& label = Node.getComponent<labelRendererComponent>();
			if (ImGui::TreeNodeEx((void*)typeid(labelRendererComponent).hash_code(), 0, "label"))
			{
				ImGui::DragFloat4("color", glm::value_ptr(label.color), 0.25f);
				char buffer[256];
				memset(buffer, 0, sizeof(buffer));
				strcpy_s(buffer, label.filePath.c_str());
				if (ImGui::InputText("file path", buffer, sizeof(buffer))) label.filePath = std::string(buffer);
				ImGui::SameLine();
				if (ImGui::Button("select image"))
				{
					//hotpink color code (227,28,121)
					label.filePath = luna::platform::os::openFilaDialog("font (*.ttf)\0*.ttf\0");
					label.font = renderer::font::create(label.filePath);
				}
				char labelBuffer[256];
				memset(labelBuffer, 0, sizeof(labelBuffer));
				strcpy_s(labelBuffer, label.text.c_str());
				if (ImGui::InputText("label text", labelBuffer, sizeof(labelBuffer))) label.text = std::string(labelBuffer);
				ImGui::TreePop();
			}
			ImGui::Separator();
		}
	}


	void sceneHierarchyPanel::drawNodeSelectionList()
	{
		ImGuiTreeNodeFlags flags = (m_ListSelected == node) ? ImGuiTreeNodeFlags_Selected : 0;
		bool isOpenNode = ImGui::TreeNodeEx("node", ImGuiTreeNodeFlags_OpenOnArrow | flags, "node");
		if(isOpenNode)
		{
			if (ImGui::IsItemClicked()) m_ListSelected = node;
			ImGui::TreePop();
		}

		flags = (m_ListSelected == controlNode) ? ImGuiTreeNodeFlags_Selected : 0;
		if (ImGui::TreeNodeEx("control node", ImGuiTreeNodeFlags_OpenOnArrow | flags, "control node"))
		{
			
			if (ImGui::IsItemClicked()) m_ListSelected = controlNode;
			
			ImGui::Indent(10);
			flags = (m_ListSelected == (spriteNode || labelNode)) ? ImGuiTreeNodeFlags_Selected : 0;
			if (ImGui::TreeNodeEx("spriteNode", ImGuiTreeNodeFlags_OpenOnArrow | flags, "spriteNode"))
			{
				if (ImGui::IsItemClicked()) m_ListSelected = spriteNode;
				ImGui::TreePop();
			}

			if (ImGui::TreeNodeEx("labelNode", ImGuiTreeNodeFlags_OpenOnArrow | flags, "labelNode"))
			{
				if (ImGui::IsItemClicked()) m_ListSelected = labelNode;
				ImGui::TreePop();
			}
			ImGui::Indent(0);
			ImGui::TreePop();

		}
	}
}