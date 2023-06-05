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
	}
}
