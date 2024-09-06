#include "object.h"
#include <core/scene/scene.h>
namespace luna
{

	void object::init(luna::scene* scene)
	{

		p_scene = scene;
		entityHandle = scene->create();

		addComponent<idComponent>().typeName = stringify(object);
		addComponent<signalComponent>();
		//LN_EMIT_SIGNAL("_ReadyEventHandler");
	}

	void object::bindMethods()
	{

	}


	void object::emitSignalParams(const char* functionName, void** monoParams)
	{
		auto it = getComponent<signalComponent>().connectedSignals.find(functionName);
		if (it != getComponent<signalComponent>().connectedSignals.end()) {
			if (monoParams) {
				for (const auto& targetNodeId : it->second) {
					object targetNode = { (entt::entity)targetNodeId.connectedObj, p_scene };
					targetNode.getComponent<scriptComponent>().scritpInstance->invokeSignal(targetNodeId, monoParams);
				}
				return;
			}
			for (const auto& targetNodeId : it->second) {
				object targetNode = { (entt::entity)targetNodeId.connectedObj, p_scene };
				targetNode.getComponent<scriptComponent>().scritpInstance->invokeSignal(targetNodeId, nullptr);
			}
		}
	}

	void object::connectSignal(uint64_t objectID, const std::string& signalName)
	{
	

	}

	std::vector<std::string> object::getSignalNames()
	{
		
	}

	uuid object::getUUID()
	{
		 return getComponent<idComponent>().id; 
	}

}