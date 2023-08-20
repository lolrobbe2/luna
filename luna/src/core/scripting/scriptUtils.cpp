#include "scriptUtils.h"
#include <core/scripting/scriptingEngine.h>
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
		void scriptInstance::invokeSignal(std::string& signalName, void** params)
		{
		}
		void scriptInstance::connectSignal(const signal& signal, MonoObject* object)
		{
			auto& mapIter = connectedSignals.find(signal.signalName);
			if(mapIter != connectedSignals.end()) // found signal by name
			{
				auto& vectorIter = std::find_if(mapIter->second.begin(), mapIter->second.end(), [&](connectedSignal signal) {
					return signal.connectedObj == object;
					});
				if (vectorIter == mapIter->second.end())
				{
					MonoMethod* signalImplementation = mono_class_get_method_from_name(m_ScriptClass->childClass, signal.signalName.c_str(), signal.paramCount);
					//MonoMethodSignature* signalSignature = scripting::scriptingEngine::getSignature(signalImplementation);
					if (scripting::scriptingEngine::hasFlag(signalImplementation, MONO_METHOD_ATTR_VIRTUAL)) {
						mapIter->second.push_back({ object,signal.signalMethod });
					}
				}
			}
		}
	}
}
