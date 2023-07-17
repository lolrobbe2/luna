#pragma once
#include <core/core.h>
#include <core/debug/uuid.h>

namespace luna
{
	namespace scripting
	{
		class scriptClass;
	}
	class scene;
	namespace utils 
	{
		
		class LN_API scriptUtils
		{
		public:
			static std::vector<const char*> getAppClassNames();
			static void setContext(scene* scene);
			static void reloadAssamblies();
		};

		class scriptInstance {
		public:
			scriptInstance(scripting::scriptClass* scriptClass, uint32_t entityHandle);
			~scriptInstance();
			void ready();
			void process(float deltaTime);
			void invokeSignal(std::string& signalName, void** params);
			MonoObject* getInstance() { return instance; };
		private: 
			scripting::scriptClass* m_ScriptClass;
			uint32_t handle;
			uuid entityId;
			MonoObject* instance;
		};
	}
}


