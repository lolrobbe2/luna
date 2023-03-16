#include "scene.h"
#include <core/rendering/renderer2D.h>
#include <core/events/mouseEvent.h>
namespace luna
{

	
	static void draw(Node node)
	{
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
			if (sprite.texture) renderer::renderer2D::drawQuad(transform.translation, { transform.scale.x,transform.scale.y }, sprite.texture);	
		}
		if(node.hasComponent<labelRendererComponent>())
		{
			auto& label = node.getComponent<labelRendererComponent>();
			if (label.font) renderer::renderer2D::drawLabel(transform.translation, { transform.scale.x,transform.scale.y }, label.font, label.text);
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
		/*
		auto itemListGroup = m_Registry.view<transformComponent, spriteRendererComponent>();
		for (auto entity : itemListGroup)
		{
			auto [transform, button, sprite] = buttonGroup.get<transformComponent, buttonComponent, spriteRendererComponent>(entity);

		}
		auto buttonGroup = m_Registry.view<transformComponent, buttonComponent,spriteRendererComponent>();
		for (auto entity : buttonGroup)
		{
			auto [transform, button,sprite] = buttonGroup.get<transformComponent, buttonComponent,spriteRendererComponent>(entity);
			if (button.hover && button.pressed) sprite.texture = button.pressedTexture;
			else if (button.hover && !button.pressed) sprite.texture = button.hoverTexture;
			else sprite.texture = button.normalTexture;
			if (sprite.texture) renderer::renderer2D::drawQuad(transform.translation, { transform.scale.x,transform.scale.y }, sprite.texture);
		}

		auto spriteGroup = m_Registry.view<transformComponent,spriteRendererComponent>();		
		for (auto entity : spriteGroup)
		{
			auto [transform, sprite] = spriteGroup.get<transformComponent, spriteRendererComponent>(entity);
			if(sprite.texture) renderer::renderer2D::drawQuad(transform.translation,{transform.scale.x,transform.scale.y}, sprite.texture);
		}

		auto labelGroup = m_Registry.view<transformComponent,labelRendererComponent>();
		//group.sort<transformComponent>([](const auto& transform1, const auto& transform2) {return transform1.translation.z > transform2.translation.z; });

		for (auto labelEntity : labelGroup)
		{
			auto [transform, label] = labelGroup.get<transformComponent, labelRendererComponent>(labelEntity);
			if (label.font) renderer::renderer2D::drawLabel(transform.translation, { transform.scale.x,transform.scale.y },label.font,label.text);
		}
		*/
	}

	void scene::onUpdate(utils::timestep ts)
	{
		auto buttonGroup = m_Registry.view<transformComponent, buttonComponent, spriteRendererComponent>();
		for (auto entity : buttonGroup)
		{
			glm::vec2 normailizedMousePos = renderer::renderer::getSceneMousePos() / renderer::renderer::getSceneDimensions();
			if (normailizedMousePos.x > 0.5f) normailizedMousePos.x -= 0.5f;
			else normailizedMousePos.x = -0.5f + normailizedMousePos.x;
			if (normailizedMousePos.y > 0.5f) normailizedMousePos.y -= 0.5f;
			else normailizedMousePos.y = -0.5f + normailizedMousePos.y;

			auto [transform, button, sprite] = buttonGroup.get<transformComponent, buttonComponent, spriteRendererComponent>(entity);
			glm::vec2 leftCorner = { transform.translation.x - transform.scale.x / 2.0f,transform.translation.y - transform.scale.y / 2.0f};
			glm::vec2 rightCorner = { transform.translation.x + transform.scale.x / 2.0f,transform.translation.y + transform.scale.y / 2.0f};
			leftCorner /= 2.0f; //origin coordinates are in center!
			rightCorner /= 2.0f;//origin coordinates are in center!

			button.hover = (leftCorner.x < normailizedMousePos.x&& leftCorner.y < normailizedMousePos.y&& rightCorner.x > normailizedMousePos.x&& rightCorner.y > normailizedMousePos.y);
		}
	}
	void scene::onEvent(Event& event)
	{
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
	}
	Node::Node(uint64_t id, luna::scene* scene)
	{
		auto idComponents = scene->m_Registry.group<idComponent,tagComponent>();
		this->scene = scene;
		
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
		: entityHandle(handle), scene(scene)
	{
	}
	Node::Node(luna::scene* scene)
	{
		this->scene = scene;
		entityHandle = scene->create();
		addComponent<idComponent>();
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

	void Node::init(luna::scene* scene)
	{
		this->scene = scene;
		entityHandle = scene->create();
		
		addComponent<idComponent>().typeName = LN_CLASS_STRINGIFY(Node);
		LN_CORE_INFO("node uuid = {0}", getUUID().getId());
	}
}
