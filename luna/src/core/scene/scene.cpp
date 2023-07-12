#include "scene.h"
#include <core/rendering/renderer2D.h>
#include <core/events/mouseEvent.h>
#include <nodes/controlNodes/itemListNode.h>
#include <core/object/methodDB.h>
#include <core/scripting/scriptingEngine.h>
#include <queue>
namespace luna
{

	
	//Node implmentation
	static void draw(Node node)
	{
		LN_PROFILE_FUNCTION();
		std::vector<Node> childNodes;
		if (node.hasComponent<childComponent>()) for (auto child : node.getComponent<childComponent>().childs) childNodes.push_back(Node(child, node));
		if (!node.hasComponent<transformComponent>()) { for (Node child : childNodes) draw(child); return; }
		auto& transform = node.getComponent<transformComponent>();
		if(node.hasComponent<spriteRendererComponent>())
		{
			auto& sprite = node.getComponent<spriteRendererComponent>();
			
			if(node.hasComponent<buttonComponent>())
			{

				auto& button = node.getComponent<buttonComponent>();
				if (button.hover && button.pressed) sprite.texture = button.pressedTexture;
				else if (button.hover && !button.pressed) sprite.texture = button.hoverTexture;
				else sprite.texture = button.normalTexture;
			}
			if (sprite.texture) sprite.outOfBounds = renderer::renderer2D::drawQuad(transform.translation, { transform.scale.x,transform.scale.y }, sprite.texture);	
		}
		if(node.hasComponent<rectComponent>())
		{
			auto& rect = node.getComponent<rectComponent>();
			renderer::renderer2D::drawQuad(transform.translation, transform.scale, rect.color);
		}
		if(node.hasComponent<labelRendererComponent>())
		{
			auto& label = node.getComponent<labelRendererComponent>();
			if (label.font) renderer::renderer2D::drawLabel(transform.translation, { transform.scale.x,transform.scale.y }, label.font, label.text);
		}
		if (node.hasComponent<itemList>())
		{
			auto& itemListComponent = node.getComponent<itemList>();
			glm::vec3 translation {0.0f,0.0f,0.0f};
			glm::vec3 customTransform = { 0.0f,0.0f,0.0f };
			static glm::vec2 glyphDimensions;
			if (itemListComponent.font) glyphDimensions = glm::vec2(itemListComponent.font->getGlyph('A')->getWidth(), itemListComponent.font->getGlyph('A')->getHeight());
			glm::vec2 advance{};
			advance.x = (glyphDimensions.x / renderer::renderer::getSceneDimensions().x) + transform.translation.x;
			advance.y = (glyphDimensions.y / renderer::renderer::getSceneDimensions().y) + transform.translation.y;
			for(item& item : itemListComponent.items)
			{
				glm::vec2 size{ 15,3 };
				size.x *= transform.scale.x;
				size.y *= transform.scale.y;
				if (renderer::renderer2D::drawQuad(translation + transform.translation, size + glm::vec2(0.01f), item.customBg))
				{ 
					item.selectable = false;
					return;
				} else item.selectable = true;;
				
				renderer::renderer2D::drawQuad(translation + transform.translation,size, item.customFg);
				customTransform = translation;
				customTransform = customTransform - glm::vec3(size.x / 2,-size.y / 4.0f, 0.0f);
				if (itemListComponent.font) renderer::renderer2D::drawLabel(customTransform + transform.translation, { transform.scale.x,transform.scale.y }, itemListComponent.font, item.text);
				item.rectCache.start = size;
				item.rectCache.position = translation;
				translation.y += size.y;
			}
		}
		for (Node child : childNodes) draw(child);
	}

	scene::~scene() 
	{
		m_Registry.clear();
	}

	template<typename T>
	bool scene::destroyNode(const T& Node)
	{
		enttityStorage.eraseValue(Node.getUUID());
		m_Registry.destroy(Node);
		return false;
	}

	void scene::onUpdateEditor(utils::timestep ts)
	{
		scene* _scene = this;
		m_Registry.each([&](auto entityID)
			{
				Node Node{ entityID ,  _scene};
				if (!m_Registry.all_of<parentComponent>(entityID)) draw(Node);
			});
	}

	void scene::onUpdate(utils::timestep ts)
	{
		if (!m_IsRunning) return;

		process(ts);

		glm::vec2 normailizedMousePos = renderer::renderer::getSceneMousePos() / renderer::renderer::getSceneDimensions();
		if (normailizedMousePos.x > 0.5f) normailizedMousePos.x -= 0.5f;
		else normailizedMousePos.x = -0.5f + normailizedMousePos.x;
		if (normailizedMousePos.y > 0.5f) normailizedMousePos.y -= 0.5f;
		else normailizedMousePos.y = -0.5f + normailizedMousePos.y;


		auto buttonGroup = m_Registry.view<transformComponent, buttonComponent, spriteRendererComponent>();
		for (auto entity : buttonGroup)
		{
			auto [transform, button, sprite] = buttonGroup.get<transformComponent, buttonComponent, spriteRendererComponent>(entity);
			if (sprite.outOfBounds) break;

			glm::vec2 leftCorner = { transform.translation.x - transform.scale.x / 2.0f,transform.translation.y - transform.scale.y / 2.0f };
			glm::vec2 rightCorner = { transform.translation.x + transform.scale.x / 2.0f,transform.translation.y + transform.scale.y / 2.0f };
			leftCorner /= 2.0f; //origin coordinates are in center!
			rightCorner /= 2.0f;//origin coordinates are in center!

			button.hover = (leftCorner.x < normailizedMousePos.x&& leftCorner.y < normailizedMousePos.y&& rightCorner.x > normailizedMousePos.x&& rightCorner.y > normailizedMousePos.y);
		}

		auto itemListGroup = m_Registry.view<transformComponent, itemList>();

		for (auto entity : itemListGroup)
		{
			int index = 0;

			bool found = false;
			auto [transform,itemListComponent] = itemListGroup.get<transformComponent, itemList>(entity);
			for (item& item : itemListComponent.items) {
				if (!item.selectable) break;
				glm::vec2 leftCorner = { transform.translation.x + item.rectCache.position.x - item.rectCache.start.x / 2.0f,transform.translation.y + item.rectCache.position.y - item.rectCache.start.y / 2.0f };
				glm::vec2 rightCorner = { transform.translation.x + item.rectCache.position.x + item.rectCache.start.x / 2.0f,transform.translation.y + item.rectCache.position.y + item.rectCache.start.y / 2.0f };
				leftCorner /= 2.0f; //origin coordinates are in center!
				rightCorner /= 2.0f;//origin coordinates are in center!
				item.hover = (leftCorner.x < normailizedMousePos.x&& leftCorner.y < normailizedMousePos.y&& rightCorner.x > normailizedMousePos.x&& rightCorner.y > normailizedMousePos.y);
				if (item.hover)
				{
					itemListComponent.current = index;
					found = true;
				}
				index++;
			}
			if (!found) itemListComponent.current = -1;
		}
	}

	void scene::onEvent(Event& event)
	{
		if (!m_IsRunning) return;
		if(event.getEventType() == eventType::MouseButtonPressed)
		{
			mouseButtonPressedEvent* mouseEvent = (mouseButtonPressedEvent*)&event;
			if (mouseEvent->getMouseButton() == Mouse::ButtonLeft) {
				auto buttonComponentGroup = m_Registry.view<buttonComponent,transformComponent>();
				for (auto entity : buttonComponentGroup)
				{
					auto [button,transform] =  buttonComponentGroup.get<buttonComponent,transformComponent>(entity);
					button.pressed = true;
				}
			}
		} 
		else if(event.getEventType() == eventType::MouseButtonReleased)
		{
			mouseButtonPressedEvent* mouseEvent = (mouseButtonPressedEvent*)&event;
			if (mouseEvent->getMouseButton() == Mouse::ButtonLeft) 
			{
				auto buttonComponentGroup = m_Registry.view<buttonComponent, transformComponent>();
				for (auto entity : buttonComponentGroup)
				{
					auto [button, transform] = buttonComponentGroup.get<buttonComponent, transformComponent>(entity);
					button.pressed = false;
				}
			}
		}
		auto itemLists = m_Registry.view<itemList>();
		for (auto entity : itemLists)
		{
			nodes::itemListNode node(entity, this);
			node.guiInputEvent(event);
		}

	}

	void scene::onPlayScene()
	{
		auto scriptComponents = m_Registry.view<scriptComponent,idComponent>();
		for (auto entity : scriptComponents)
		{
			auto& script = m_Registry.get<scriptComponent>(entity);
			if (script.scritpInstance) LN_CORE_ERROR("scriptInstance was not nullptr");
			else if (script.className.size()) script.scritpInstance = new utils::scriptInstance(scripting::scriptingEngine::getScriptClass(script.className), (uint32_t)entity);
		}
		for (auto entity : scriptComponents)
		{
			auto& script = m_Registry.get<scriptComponent>(entity);
			if (script.scritpInstance) script.scritpInstance->ready();
		}
	}

	void scene::onStopScene()
	{
		auto scriptComponents = m_Registry.view<scriptComponent, idComponent>();
		for (auto entity : scriptComponents)
		{
			auto& script = m_Registry.get<scriptComponent>(entity);
			script.scritpInstance = nullptr;
		
		}
	}

	void scene::resetScriptComponent()
	{
		auto scriptComponents = m_Registry.view<scriptComponent, idComponent>();

		auto appClassNames = utils::scriptUtils::getAppClassNames();
		std::vector<std::string> stringAppClassNames;
		stringAppClassNames.reserve(appClassNames.size()); // Reserve memory for efficiency

		for (const char* c : appClassNames) {
			stringAppClassNames.emplace_back(c); // Construct std::string from const char* and add it to the vector
		}

		for (auto entity : scriptComponents)
		{
			auto& script = m_Registry.get<scriptComponent>(entity);
	
			auto it = std::find(stringAppClassNames.begin(), stringAppClassNames.end(), script.className);
			if (it != stringAppClassNames.end())
				script.currentItem = std::distance(stringAppClassNames.begin(), it);
			else script.currentItem = -1;
		}
	}

	void scene::process(utils::timestep ts)
	{
		// Create a queue to store entities
		std::queue<entt::entity> entityQueue;

		// Add the initial entities to the queue
		auto view = m_Registry.view<idComponent, tagComponent>(entt::exclude<parentComponent>);
		for (auto entityID : view) {
			Node node{ entityID, this };
			if (node.hasComponent<scriptComponent>() && node.getComponent<scriptComponent>().scritpInstance) {
				node.getComponent<scriptComponent>().scritpInstance->process(ts);
			}
			// Check if the entity has child entities
			auto childView = m_Registry.view<parentComponent>();
			if (!childView.empty()) {
				entityQueue.push(entityID);
			}
		}

		// Process entities in a breadth-first manner
		while (!entityQueue.empty()) {
			entt::entity entityID = entityQueue.front();
			entityQueue.pop();

			Node node{ entityID, this };

			// Check if the entity has a childComponent
			if (node.hasComponent<childComponent>()) {
				auto& children = node.getComponent<childComponent>().childs;
				for (auto childEntityID : children) {
					Node childNode{ childEntityID, this };
					if (childNode.hasComponent<scriptComponent>()) {
						childNode.getComponent<scriptComponent>().scritpInstance->process(ts);
					}
					entityQueue.push(childEntityID);
				}
			}
		}
	}

#pragma region NODE

	/*-----------------------------------------------------------------------*/
	/*                                glue                                 */
	/*-----------------------------------------------------------------------*/


	static void NodeSetName(entt::entity nodeHandle, MonoString* name)
	{
		Node node = { nodeHandle,scripting::scriptingEngine::getContext() };
		node.setName(mono_string_to_utf8(name));
	}

	static void NodeGetName(entt::entity nodeHandle, MonoString** name)
	{
		Node node = { nodeHandle,scripting::scriptingEngine::getContext() };
		*name = scripting::scriptingEngine::createMonoString(node.getName());
	}
	static MonoArray* NodeGetChildren(entt::entity nodeId)
	{
		Node node = { nodeId,scripting::scriptingEngine::getContext() };
		auto children = node.getChildren();

		MonoArray* nodeArray = scripting::scriptingEngine::createArray<Node>(children.size());
		for (size_t i = 0; i < children.size(); i++)
		{
			auto& script = children[i].getComponent<scriptComponent>();
			MonoObject* nodeObject = script.scritpInstance->getInstance();
			mono_array_set(nodeArray, MonoObject*, i, nodeObject);
		}

		return nodeArray;
	}

	static MonoObject* NodeGetParent(entt::entity nodeId)
	{
		Node node = { nodeId,scripting::scriptingEngine::getContext() };
		Node parent = node.getParent();
		if (parent) {
			return parent.getComponent<scriptComponent>().scritpInstance->getInstance();
		}
		return nullptr;
	}

	static void NodeAddSibling(entt::entity nodeId, entt::entity siblingId)
	{
		Node sibling = { nodeId,scripting::scriptingEngine::getContext() };
		sibling.getParent().addChild(Node(siblingId, scripting::scriptingEngine::getContext()));
	}

	static void NodeAddChild(entt::entity nodeId, entt::entity childId)
	{
		Node parent = { nodeId,scripting::scriptingEngine::getContext() };
		parent.addChild(Node(childId, scripting::scriptingEngine::getContext()));
	}

	static entt::entity NodeCreateNew()
	{
		return Node(scripting::scriptingEngine::getContext());
	}
	Node::Node(uint64_t id, luna::scene* scene)
	{
		auto idComponents = scene->m_Registry.group<idComponent,tagComponent>();
		this->scene = scene;
		if (id == -1) return;
		for (auto entity : idComponents)
		{
			auto [testId,tag] = scene->m_Registry.get<idComponent, tagComponent>(entity);
			if (testId.id == id) {
				entityHandle = entity;
				if (!hasComponent<idComponent>()) addComponent<idComponent>().id = id;
				return;
			}
		}
	}

	Node::Node(entt::entity handle, luna::scene* scene)
		: object(handle,scene)
	{
	}

	Node::Node(luna::scene* scene)
	{
		this->scene = scene;
		entityHandle = scene->create();
		addComponent<idComponent>();
		addComponent<scriptComponent>();
		LN_CORE_INFO("node uuid = {0}", getUUID().getId());
	}

	void Node::setName(std::string name)
	{
		if (hasComponent<tagComponent>()) getComponent<tagComponent>().tag = name;
		else addComponent<tagComponent>(name);
		
	}

	void Node::addChild(Node node)
	{
		LN_CORE_INFO("adding node {0} as a child to {1} .", node.getUUID().getId(), getUUID().getId());
		if (node.hasComponent<parentComponent>()) node.getComponent<parentComponent>().parentId = getComponent<idComponent>().id;
		else node.addComponent<parentComponent>().parentId = getComponent<idComponent>().id;
	
		if (hasComponent<childComponent>()) getComponent<childComponent>().childs.push_back(node);
		else addComponent<childComponent>().childs.push_back(node);
	}

	std::vector<Node> Node::getChildren()
	{
		std::vector<Node> children;
		if (hasComponent<childComponent>())
		{
			auto childrenID = getComponent<childComponent>().childs;
			for (auto child : childrenID) {
				children.push_back(Node(child, scene));
			}
		}
		return children;
	}

	Node Node::getParent() 
	{
		if (hasComponent<parentComponent>()) {
			auto& parentComp = getComponent<parentComponent>();
			Node parent{ parentComp.parentId,scene };
			return parent;
		}
		return Node(-1,scene);
	}

	void Node::init(luna::scene* scene)
	{
		this->scene = scene;
		entityHandle = scene->create();
		
		addComponent<idComponent>().typeName = LN_CLASS_STRINGIFY(Node);
		addComponent<scriptComponent>();
		LN_CORE_INFO("node uuid = {0}", getUUID().getId());
	}

	void Node::bindMethods() 
	{
		LN_ADD_INTERNAL_CALL(Node, NodeSetName);
		LN_ADD_INTERNAL_CALL(Node, NodeGetName);
		LN_ADD_INTERNAL_CALL(Node, NodeGetChildren);
		LN_ADD_INTERNAL_CALL(Node, NodeGetParent);
		LN_ADD_INTERNAL_CALL(Node, NodeAddSibling);
		LN_ADD_INTERNAL_CALL(Node, NodeAddChild);
		LN_ADD_INTERNAL_CALL(Node, NodeCreateNew);
	}

#pragma endregion

}


