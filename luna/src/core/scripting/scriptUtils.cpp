#include "scriptUtils.h"
#include <core/scripting/scriptingEngine.h>
#include <core/object/objectDB.h>
namespace luna
{
	namespace utils
	{
		std::vector<const char*> scriptUtils::getAppClassNames()
		{
			return scripting::scriptingEngine::getAppClassNames();
		}

		void scriptUtils::setContext(scene* scene)
		{
			scripting::scriptingEngine::setContext(scene);
		}

		void scriptUtils::reloadAssamblies()
		{
			scripting::scriptingEngine::reloadAssembly();
		}
		MonoString* scriptUtils::createMonoString(const std::string& string)
		{
			return mono_string_new(mono_get_root_domain(), string.c_str());
		}
		scriptInstance::scriptInstance(scripting::scriptClass* scriptClass, uint32_t entityHandle) : m_ScriptClass(scriptClass) , handle(entityHandle)
		{
			LN_CORE_INFO("instancing class: {0}",name());
			instance = m_ScriptClass->childClass.instanciate();
			void* param = &handle;
			object((entt::entity)entityHandle, scripting::scriptingEngine::getContext()).getComponent<scriptComponent>().scritpInstance = this;
			
			instance->invoke(m_ScriptClass->constructor,&param);
			object((entt::entity)entityHandle, scripting::scriptingEngine::getContext()).emitSignal("TreeEntered");
		}

		scriptInstance::~scriptInstance() {
			//delete instance;
		}
		void scriptInstance::ready()
		{
			instance->invoke(m_ScriptClass->readyMethod, nullptr);
		}
		void scriptInstance::process(float deltaTime)
		{
			void* param = &deltaTime;
			instance->invoke(m_ScriptClass->processMethod, &param);
		}
		void scriptInstance::invokeSignal(const connectedSignal& signal, void** params)
		{
			LN_ERR_FAIL_COND_MSG(signal.connectedObj == 0, "[scripting] signals must be connected to a Node to be invoked!");
			LN_ERR_FAIL_NULL_MSG(signal.signalMethodPtr, "[scripting] no singal function callback has been found");

			instance->invoke(signal.signalMethodPtr, params);
		}
		void scriptInstance::connectSignal(const signal& signal,uint64_t entity)
		{
			/*
			MonoMethod* signalImplementation = mono_class_get_method_from_name(m_ScriptClass->childClass, signal.signalName.c_str(), signal.paramCount);
			if (scripting::scriptingEngine::hasFlag(signalImplementation, MONO_METHOD_ATTR_VIRTUAL)) {
				mapIter->second.push_back({ entity,signal.signalMethod });
			}
			*/

		}
		std::string scriptInstance::name()
		{
			return m_ScriptClass->childClass.getName();
		}
		MonoClass* scriptInstance::getClass()
		{
			return m_ScriptClass->childClass.getNative();
		}
	}
}
