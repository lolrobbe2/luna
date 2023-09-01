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
	
	object::object(uint64_t id, luna::scene* scene) : scene(scene)
	{
		auto idComponents = scene->m_Registry.view<idComponent, tagComponent>();
		if (id == -1) return;
		for (auto entity : idComponents)
		{
			auto [testId, tag] = scene->m_Registry.get<idComponent, tagComponent>(entity);
			if (testId.id == id) {
				entityHandle = entity;
				if (!hasComponent<idComponent>()) addComponent<idComponent>().id = id;
				return;
			}
		}
	}

	void object::init(luna::scene* scene)
	{
		addComponent<idComponent>().typeName = stringify(object);
		addComponent<signalComponent>();
		LN_EMIT_SIGNAL("_ReadyEventHandler");
	}

	void object::bindMethods()
	{
		
	}


	void object::connectSignal(uint64_t objectID,const std::string& signalName)
	{
		const signal& signal = signalDB::getSignalByName(getComponent<idComponent>().typeName, signalName);
		auto& connectedSignals = getComponent<signalComponent>().connectedSignals;

		const std::string& className = object((entt::entity)objectID, scripting::scriptingEngine::getContext()).getComponent<scriptComponent>().className;
		
		MonoClass* childClass = nullptr;
		if(className != "") childClass = scripting::scriptingEngine::getScriptClass(className)->childClass;

		if (!connectedSignals.empty()) {
			auto mapIter = connectedSignals.find(signal.signalName); //signalName already registered/ has connections
			if (mapIter != connectedSignals.end())
			{
				auto vecIter = std::find_if(mapIter->second.begin(), mapIter->second.end(), [&](connectedSignal signal) {return signal.connectedObj != objectID; });
				if (vecIter != mapIter->second.end())
					return mapIter->second.push_back({ objectID,mono_class_get_method_from_name(childClass,signal.signalName.c_str(),signal.paramCount) });
				
				LN_CORE_ERROR("signal {0} already connected to node {1}", signal.signalName, object((entt::entity)objectID, scripting::scriptingEngine::getContext()).getComponent<idComponent>().id.getId());
			}
		}
		std::vector<connectedSignal> signals;
		if (childClass) 
		{
			signals.push_back({ objectID, mono_class_get_method_from_name(childClass,signal.signalName.c_str(),signal.paramCount) });
			connectedSignals.insert({ signal.signalName,signals });
			return;
		}
		signals.push_back({ objectID,nullptr });
		connectedSignals.insert({ signal.signalName,signals });

	}

	std::vector<std::string> object::getSignalNames()
	{
		return signalDB::getSignalNames(getComponent<idComponent>().typeName);
	}

	template<typename T, typename... Args>
	T& object::addOrReplaceComponent(Args&&... args)
	{
		T& component = scene->m_Registry.emplace_or_replace<T>(entityHandle, std::forward<Args>(args)...);
		//scene->onComponentAdded<T>(*this, component);
		return component;
	}
}
