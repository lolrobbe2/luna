#pragma once
#include <core/core.h>
#include <core/debug/uuid.h>
#include <core/object/signal.h>

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
			void invokeSignal(const connectedSignal& signal, void** params);
			void connectSignal(const signal& signal, uint64_t entity);
			MonoObject* getInstance() { return instance; };
			MonoClass* getClass();
		private: 
			scripting::scriptClass* m_ScriptClass;
			uint32_t handle;
			uuid entityId;
			MonoObject* instance;
			std::unordered_map<std::string,std::vector<connectedSignal>> connectedSignals;
		};
	}
}


