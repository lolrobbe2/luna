#include "scene.h"
#include <core/rendering/renderer2D.h>
namespace luna
{

	template<typename T>
	bool scene::destroyNode(const T& Node)
	{
		enttityStorage.eraseValue(node.getUUID());
		m_Registry.destroy(node);
		return false;
	}

	void scene::onUpdateEditor(utils::timestep ts)
	{

		auto buttonGroup = m_Registry.view<transformComponent, buttonComponent,spriteRendererComponent>();
		for (auto entity : buttonGroup)
		{
			auto [transform, button,sprite] = buttonGroup.get<transformComponent, buttonComponent,spriteRendererComponent>(entity);
			if (button.state < 0) sprite.texture = button.pressedTexture;
			else if (button.state > 0) sprite.texture = button.normalTexture;
			else sprite.texture = button.hoverTexture;
			if (sprite.texture) renderer::renderer2D::drawQuad(transform.translation, { transform.scale.x,transform.scale.y }, sprite.texture);
			


		}

		auto spriteGroup = m_Registry.view<transformComponent,spriteRendererComponent>();
		//group.sort<transformComponent>([](const auto& transform1, const auto& transform2) {return transform1.translation.z > transform2.translation.z; });
		
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
			
			glm::vec2 left = { transform.translation.x - transform.scale.x / 2.0f,transform.translation.y - transform.scale.y / 2.0f};
			glm::vec2 right = { transform.translation.x + transform.scale.x / 2.0f,transform.translation.y + transform.scale.y / 2.0f};
			left /= 2.0f; //origin coordinates are in center!
			right /= 2.0f;//origin coordinates are in center!

			if(left.x < normailizedMousePos.x && left.y < normailizedMousePos.y && right.x > normailizedMousePos.x && right.y > normailizedMousePos.y)
			{
				LN_CORE_INFO("hover");
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
		addComponent<idComponent>();
		LN_CORE_INFO("node uuid = {0}", getUUID().getId());
	}
}
