#include "scene.h"
namespace luna
{

	template<typename T>
	bool scene::deleteNode(const T& node)
	{
		return false;
	}
	template<typename T>
	void scene::onComponentAdded(luna::node node, T& component) 
	{

	}

	node::node(luna::scene* scene)
	{
		this->scene = scene;
		entityHandle = scene->m_Registry.create();
		addComponent<idComponent>();
	}
	void node::setName(std::string name)
	{
		if (hasComponent<tagComponent>()) getComponent<tagComponent>().tag = name;
		else 
		{
			addComponent<tagComponent>(name);
		}
	}

	template<typename T, typename... Args>
	T& node::addComponent(Args&&... args)
	{
		//LN_CORE_ASSERT(!hasComponent<T>(), "node already has component!");
		T& component = scene->m_Registry.emplace<T>(entityHandle, std::forward<Args>(args)...);
		scene->onComponentAdded<T>(*this, component);
		return component;
	}

	template<typename T, typename... Args>
	T& node::addOrReplaceComponent(Args&&... args)
	{
		T& component = scene->m_Registry.emplace_or_replace<T>(entityHandle, std::forward<Args>(args)...);
		scene->onComponentAdded<T>(*this, component);
		return component;
	}

	template<typename T>
	T& node::getComponent()
	{
		//LN_CORE_ASSERT(hasComponent<T>(), "node does not have component!");
		return scene->m_Registry.get<T>(entityHandle);
	}
	template<typename T>
	bool node::hasComponent()
	{
		return scene->m_Registry.all_of<T>(entityHandle);
	}
}
