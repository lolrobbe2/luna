#include "objectDB.h"
#include <core/scene/scene.h>
#include <core/scripting/scriptingEngine.h>
namespace luna
{

	void objectDB::createInstance(const std::string& className, scene* scene)
	{
		classInfo* info = getPtr(className);
		if (!info) return;
		object* node = (object*)info->creation_func();
		node->init(scene);
		if (node->hasComponent<tagComponent>()) node->getComponent<tagComponent>().tag = className;
		else node->addComponent<tagComponent>(className);
	}
	
	object::object(uint64_t id, luna::scene* scene)
	{
	}

	void object::init(luna::scene* scene)
	{
		addComponent<idComponent>().typeName = stringify(object);
		addComponent<signalComponent>();
	}

	void object::bindMethods()
	{
		
	}

	void object::emitSignal(std::string& functionName, void** params)
	{
		auto it = getComponent<signalComponent>().connectedSignals.find(functionName);
		if (it != getComponent<signalComponent>().connectedSignals.end()) {
			for (const auto& targetNodeId : it->second) {
				object targetNode = { targetNodeId, scene };
				targetNode.getComponent<scriptComponent>().scritpInstance->invokeSignal(functionName, params);
			}
		}
	}

	void object::connectSignal(uint64_t objectID, std::string& signalName)
	{
		getComponent<signalComponent>().connectedSignals[signalName].push_back(objectID);
	}

	std::vector<std::string> object::getSignalNames()
	{
		return signalDB::getSignalNames(getComponent<idComponent>().typeName).value();
	}

	template<typename T, typename... Args>
	T& object::addOrReplaceComponent(Args&&... args)
	{
		T& component = scene->m_Registry.emplace_or_replace<T>(entityHandle, std::forward<Args>(args)...);
		//scene->onComponentAdded<T>(*this, component);
		return component;
	}
}
