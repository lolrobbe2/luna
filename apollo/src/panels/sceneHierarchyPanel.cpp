#include "sceneHierarchyPanel.h"
//node includes start
#include <nodes/controlNodes/spriteNode.h>
#include <nodes/controlNodes/labelNode.h>
#include <nodes/controlNodes/buttonNode.h>
#include <nodes/controlNodes/itemListNode.h>
//node includes end
#include <core/rendering/renderer2D.h>
#include <glm/gtc/type_ptr.hpp>
#include <core/platform/platformUtils.h>
#include <core/object/objectDB.h>
#include <core/scripting/scriptUtils.h>
#include <core/assets/assetManager.h>
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
					m_ListSelected = "";
					ImGui::CloseCurrentPopup();
				}
				ImGui::SameLine(ImGui::GetContentRegionAvail().x-200);
				if(ImGui::Button("add selected node",ImVec2(200,20)))
				{
					//LN_CORE_INFO("node added: {0}",m_ListSelected);
					objectDB::createInstance(m_ListSelected, m_Context);
					m_ListSelected = "";
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
				ImGui::Indent(addIndent);
				auto& childs = Node.getComponent<childComponent>().childs;
				
				for (auto child : childs)
				{
					luna::Node _Node{ child,m_Context };
					if (_Node.getComponent<parentComponent>().parentId == Node.getComponent<idComponent>().id)
					{
						drawEntityNode(_Node, indent + addIndent);
					}
				}
				ImGui::Unindent(addIndent);
			}

			ImGui::TreePop();
		}
		

	}
	void sceneHierarchyPanel::drawComponents(Node Node)
	{
		if (!Node.scene || Node.entityHandle == entt::null ) return;
		if (Node.hasComponent<idComponent>())
		{
			auto& id = Node.getComponent<idComponent>();
			std::stringstream sstream;
			sstream << std::hex << id.id;
			std::string result = sstream.str();
			ImGui::LabelText(result.c_str(), "id");
			ImGui::Separator();

		}
		if (Node.hasComponent<scriptComponent>())
		{
			auto& script = Node.getComponent<scriptComponent>();			
			if (ImGui::TreeNodeEx((void*)typeid(luna::itemList).hash_code(), 0, "script"))
			{
				auto appClassNames = utils::scriptUtils::getAppClassNames();
				const char** items = appClassNames.data();
				int currentItem = script.currentItem;
				ImGui::LabelText("className", script.className.c_str());
				if(ImGui::Combo("select class", &currentItem, items, utils::scriptUtils::getAppClassNames().size()));
				{
					
				}
				if (currentItem != -1) {
					script.currentItem = currentItem;
					script.className = std::string(items[currentItem]);
				}
				ImGui::TreePop();
			}
			ImGui::Separator();
		}
		if(Node.hasComponent<tagComponent>())
		{
			auto& tag = Node.getComponent<tagComponent>().tag;
			inputText("name", tag);
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
			if (sprite.showInEditor)
			{
				if (ImGui::TreeNodeEx((void*)typeid(spriteRendererComponent).hash_code(), 0, "sprite"))
				{
					ImGui::DragFloat4("color", glm::value_ptr(sprite.color), 0.25f);
					//inputText("filePath", sprite.filePath);
					ImGui::SameLine();
					if (ImGui::Button("select image"))
					{
						sprite.filePath = luna::platform::os::openFileDialog("image\0*.png;*.jpeg;*.jpg\0");
						ref<assets::asset> texture = assets::assetManager::getAsset(sprite.filePath.filename().string());
						sprite.texture = std::dynamic_pointer_cast<renderer::texture>(texture);
					}

					ImGui::TreePop();
				}
				ImGui::Separator();
			}
		}
		if (Node.hasComponent<labelRendererComponent>())
		{
			auto& label = Node.getComponent<labelRendererComponent>();
			if (ImGui::TreeNodeEx((void*)typeid(labelRendererComponent).hash_code(), 0, "label"))
			{
				ImGui::DragFloat4("color", glm::value_ptr(label.color), 0.25f);
				ImGui::SameLine();
				if (ImGui::Button("select font"))
				{
					//hotpink color code (227,28,121)
					std::filesystem::path filePath = luna::platform::os::openFileDialog("font (*.ttf)\0*.ttf\0");
					if (assets::assetManager::isAssetHandleValid(filePath.filename().string())) {
						ref<assets::asset> font = assets::assetManager::getAsset(filePath.filename().string());
						label.handle = assets::assetManager::getAssetMetadata(filePath.filename().string())->handle;
						label.font = std::dynamic_pointer_cast<renderer::font>(font);
					} else {
						label.handle = assets::assetManager::importAsset(filePath.string(), assets::font);
						ref<assets::asset> font = assets::assetManager::getAsset(label.handle);
						label.handle = assets::assetManager::getAssetMetadata(filePath.filename().string())->handle;
						label.font = std::dynamic_pointer_cast<renderer::font>(font);
					}
					
				}
				inputText("label text", label.text);
				ImGui::TreePop();
			}
			ImGui::Separator();
		}
		if(Node.hasComponent<rectComponent>())
		{
			auto& rect = Node.getComponent<rectComponent>();
			if (ImGui::TreeNodeEx((void*)typeid(rectComponent).hash_code(), 0, "color rect"))
			{
				ImGui::DragFloat4("color", glm::value_ptr(rect.color), 0.1f);
				ImGui::TreePop();
			}
			ImGui::Separator();
		}
		if (Node.hasComponent<buttonComponent>())
		{
			auto& button = Node.getComponent<buttonComponent>();
			if (button.showInEditor)
			{
				if (ImGui::TreeNodeEx((void*)typeid(buttonComponent).hash_code(), 0, "button"))
				{
					if (ImGui::Button("select normal image"))
					{
						button.normalFilePath = luna::platform::os::openFileDialog("image\0*.png;*.jpeg;*.jpg\0");
						button.normalTexture = std::dynamic_pointer_cast<renderer::texture>(assets::assetManager::getAsset(button.normalFilePath.string()));
					}
					inputText("normal Image", button.normalFilePath.string());


					if (ImGui::Button("select hover image"))
					{
						button.hoverFilePath = luna::platform::os::openFileDialog("image\0*.png;*.jpeg;*.jpg\0");
						button.hoverTexture = std::dynamic_pointer_cast<renderer::texture>(assets::assetManager::getAsset(button.hoverFilePath.string()));

					}
					inputText("hover Image", button.hoverFilePath.string());

					if (ImGui::Button("select pressed image"))
					{
						button.pressedFilePath = luna::platform::os::openFileDialog("image\0*.png;*.jpeg;*.jpg\0");
						button.pressedTexture = std::dynamic_pointer_cast<renderer::texture>(assets::assetManager::getAsset(button.pressedFilePath.string()));
					}
					inputText("pressed Image", button.pressedFilePath.string());


					ImGui::TreePop();
				}
				ImGui::Separator();
			}
		}
		if (Node.hasComponent<itemList>())
		{
			auto& itemList = Node.getComponent<luna::itemList>();
			if (ImGui::TreeNodeEx((void*)typeid(luna::itemList).hash_code(), 0, "itemList"))
			{
				const char* items[] = { "single","multi" };
				static int currentItem;
					
				ImGui::Combo("select mode", &currentItem, items,2);
				
				if (currentItem == 0) itemList.selectMode = luna::itemList::SELECT_SINGLE;
				else itemList.selectMode = luna::itemList::SELECT_MULTI;
					
				if (ImGui::Button("select font"))
				{
					//hotpink color code (227,28,121)
					itemList.filePath = luna::platform::os::openFileDialog("font (*.ttf)\0*.ttf\0");
					ref<assets::asset> font = assets::assetManager::getAsset(itemList.filePath.filename().string());
					//itemList.handle = assets::assetManager::getAssetMetadata(itemList.filePath.filename().string())->handle;
					itemList.font = std::dynamic_pointer_cast<renderer::font>(font);
				}
				if (ImGui::Button("add item"))
				{
					nodes::itemListNode itemListNode(Node);
					itemListNode.addItem("test");
				}
				int itemNum = 0;
				if (ImGui::BeginChild("items"))
				{
					for (item& item : itemList.items)
					{
						std::string itemText = "item " + std::to_string(itemNum);

						if (ImGui::TreeNodeEx((void*)typeid(luna::itemList).hash_code(), 0, itemText.c_str()))
						{
							inputText(itemText, item.text);
							ImGui::TreePop();
						}
						itemNum++;
					}
				}
				ImGui::EndChild();
			
				//TODO renderer part.
				ImGui::TreePop();
			}
			ImGui::Separator();
		}

	}


	void sceneHierarchyPanel::drawNodeSelectionList()
	{
		for (auto& [key, value] : objectDB::rootClassDatabase) 
		{
			addNodeSelection(key,value);
		}
			
	}
	bool sceneHierarchyPanel::addNodeSelection(const std::string& nodeName,objectDB::classInfo* classInfo)
	{
		ImGuiTreeNodeFlags flags = (m_ListSelected == nodeName) ? ImGuiTreeNodeFlags_Selected : 0;
		bool isOpenNode = ImGui::TreeNodeEx(nodeName.c_str(), ImGuiTreeNodeFlags_OpenOnArrow | flags, nodeName.c_str());
		if (ImGui::IsItemClicked() && !ImGui::IsItemToggledOpen()) m_ListSelected = nodeName;
		if (isOpenNode) ImGui::TreePop();
		if (classInfo && isOpenNode)
		{
			ImGui::Indent(10);
			for (auto childClass : classInfo->children) addNodeSelection(childClass->className, childClass);
			ImGui::Unindent(10);
		}
	
		return isOpenNode;
	}
	void sceneHierarchyPanel::inputText(const std::string& name,std::string& stringBuffer)
	{
		char buffer[256];
		memset(buffer, 0, sizeof(buffer));
		strcpy_s(buffer, stringBuffer.c_str());
		if (ImGui::InputText(name.c_str(), buffer, sizeof(buffer))) stringBuffer = std::string(buffer);
	}
	void sceneHierarchyPanel::onPlay()
	{
		
	}
}