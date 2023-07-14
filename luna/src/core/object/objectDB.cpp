#include "objectDB.h"
#include <core/scene/scene.h>
#include <core/scripting/scriptingEngine.h>
namespace luna
{

	void objectDB::createInstance(const std::string& className, scene* scene)
	{
		classInfo* info = getPtr(className);
		if (!info) return;
		Node* node = (Node*)info->creation_func();
		node->init(scene);
		node->setName(className);
	}
	
	void object::emitSignal(std::string& functionName, void** params)
	{
		
	}

	template<typename T, typename... Args>
	T& object::addComponent(Args&&... args)
	{
		//LN_CORE_ASSERT(!hasComponent<T>(), "Node already has component!");
		T& component = scene->m_Registry.emplace<T>(entityHandle, std::forward<Args>(args)...);
		scene->onComponentAdded<T>(*this, component);
		return component;
	}

	template<typename T, typename... Args>
	T& object::addOrReplaceComponent(Args&&... args)
	{
		T& component = scene->m_Registry.emplace_or_replace<T>(entityHandle, std::forward<Args>(args)...);
		scene->onComponentAdded<T>(*this, component);
		return component;
	}

	template<typename T>
	T& object::getComponent()
	{
		return scene->m_Registry.get<T>(entityHandle);
	}

	template<typename T>
	bool object::hasComponent()
	{
		return scene->m_Registry.all_of<T>(entityHandle);
	}

	template<typename T>
	void object::removeComponent()
	{
		scene->m_Registry.remove<T>(entityHandle);
	}
}
