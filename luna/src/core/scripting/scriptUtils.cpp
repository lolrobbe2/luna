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
			scripting::scriptingEngine::secContext(scene);
		}

		void scriptUtils::reloadAssamblies()
		{
			scripting::scriptingEngine::reloadAssembly();
		}
		scriptInstance::scriptInstance(scripting::scriptClass* scriptClass, uint32_t entityHandle) : m_ScriptClass(scriptClass) , handle(entityHandle)
		{
			LN_CORE_INFO("instancing class: {0}", mono_class_get_name(m_ScriptClass->childClass));
			instance = scripting::scriptingEngine::instanciate(m_ScriptClass->childClass);
			void* param = &handle;
			mono_runtime_invoke(m_ScriptClass->constructor, instance, &param, nullptr);
		}

		scriptInstance::~scriptInstance() {
			//delete instance;
		}
		void scriptInstance::ready()
		{
			 mono_runtime_invoke(m_ScriptClass->readyMethod, instance, nullptr, nullptr);
		}
		void scriptInstance::process(float deltaTime)
		{
			void* param = &deltaTime;
			if (m_ScriptClass->processMethod) mono_runtime_invoke(m_ScriptClass->processMethod, instance,&param, nullptr);
		}
		void scriptInstance::invokeSignal(const connectedSignal& signal, void** params)
		{
			mono_runtime_invoke(signal.signalMethodPtr, instance, params, nullptr);
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
		MonoClass* scriptInstance::getClass()
		{
			return m_ScriptClass->childClass;
		}
	}
}
