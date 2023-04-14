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

		scriptInstance::scriptInstance(scripting::scriptClass* scriptClass, uuid entity)
			: m_ScriptClass(scriptClass), entityId(entity)
		{
			LN_CORE_INFO("instancing class: {0}", mono_class_get_name(m_ScriptClass->childClass));
			instance = scripting::scriptingEngine::instanciate(m_ScriptClass->childClass);
			void* param = &entityId;
			mono_runtime_invoke(m_ScriptClass->constructor, instance, &param, nullptr);
			mono_runtime_invoke(m_ScriptClass->readyMethod, instance, nullptr, nullptr);
		}

		scriptInstance::~scriptInstance() {
			//delete instance;
		}
	}
}
