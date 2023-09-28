#include "objectDB.h"
#include <core/scene/scene.h>
#include <core/scripting/scriptingEngine.h>
#include <core/debug/debugMacros.h>
namespace luna
{

	void objectDB::createInstance(const std::string& className, scene* scene)
	{
		classInfo* info = getPtr(className);
		LN_ERR_FAIL_NULL_MSG(info, "class was not found");
		object* node = (object*)info->creation_func();
		node->init(scene);
		if (node->hasComponent<tagComponent>()) node->getComponent<tagComponent>().tag = className;
		else node->addComponent<tagComponent>(className);
	}
	
	object::object(uint64_t id, luna::scene* scene) : scene(scene)
	{
		LN_ERR_FAIL_COND_MSG(id == -1, "native id was invalid");

		auto idComponents = scene->m_Registry.view<idComponent, tagComponent>();

		auto it = std::find_if(idComponents.begin(), idComponents.end(), [&](entt::entity entity) { return getComponent<idComponent>().id == id; });

		if (it != idComponents.end()) entityHandle = *it;
	}

	void object::init(luna::scene* scene)
	{

		this->scene = scene;
		entityHandle = scene->create();

		addComponent<idComponent>().typeName = stringify(object);
		addComponent<signalComponent>();
		LN_EMIT_SIGNAL("_ReadyEventHandler");
	}

	void object::bindMethods()
	{
		
	}

	void object::notification(const notificationType type)
	{
	}


	void object::connectSignal(uint64_t objectID,const std::string& signalName)
	{
		const signal& signal = signalDB::getSignalByName(getComponent<idComponent>().typeName, signalName);
		auto& connectedSignals = getComponent<signalComponent>().connectedSignals;

		const std::string& className = object((entt::entity)objectID, scripting::scriptingEngine::getContext()).getComponent<scriptComponent>().className;
		
		LN_ERR_FAIL_COND_MSG(className == "", "className was invalid! (check properties if a class was selected!)");

		MonoClass* childClass = scripting::scriptingEngine::getScriptClass(className)->childClass;

		if (!connectedSignals.empty()) {
			auto mapIter = connectedSignals.find(signal.signalName); //signalName already registered/ has connections
			if (mapIter != connectedSignals.end())
			{
				auto vecIter = std::find_if(mapIter->second.begin(), mapIter->second.end(), [&](connectedSignal signal) {return signal.connectedObj != objectID; });
				if (vecIter != mapIter->second.end())
					return mapIter->second.push_back({ objectID,mono_class_get_method_from_name(childClass,signal.signalName.c_str(),signal.paramCount) });
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


}
