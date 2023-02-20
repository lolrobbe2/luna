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
		auto group = m_Registry.group<transformComponent>(entt::get<spriteRendererComponent>);
		//group.sort<transformComponent>([](const auto& transform1, const auto& transform2) {return transform1.translation.z > transform2.translation.z; });
		
		for (auto entity : group)
		{
			auto [transform, sprite] = group.get<transformComponent, spriteRendererComponent>(entity);
			if(sprite.texture) renderer::renderer2D::drawQuad(transform.translation,{transform.scale.x,transform.scale.y}, sprite.texture);
		}
	}

	Node::Node(entt::entity handle, luna::scene* scene)
		: entityHandle(handle), scene(scene)
	{
	}
	Node::Node(luna::scene* scene)
	{
		this->scene = scene;
		entityHandle = scene->m_Registry.create();
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
}
