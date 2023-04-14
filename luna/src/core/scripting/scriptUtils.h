#pragma once
#include <core/core.h>
#include <core/debug/uuid.h>

namespace luna
{
	namespace scripting
	{
		class scriptClass;
	}
	
	namespace utils 
	{
		class LN_API scriptUtils
		{
		public:
			static std::vector<const char*> getAppClassNames();
		};

		class scriptInstance {
		public:
			scriptInstance(scripting::scriptClass* scriptClass, uuid entity);
			~scriptInstance();
		private: 
			scripting::scriptClass* m_ScriptClass;
			uuid entityId;
			void* instance;
		};
	}
}


