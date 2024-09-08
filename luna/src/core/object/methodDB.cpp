#include "methodDB.h"
#include <core/debug/log.h>
#include <core/object/signal.h>
#include <core/debug/debugMacros.h>
#include <core/utils/objectStorage.h>
//networking


namespace luna
{
	struct methodDBData
	{
		//TODO change map with hashed objStorage.
		utils::objectStorage<methodDefinition> methodDefStorage;
		utils::objectStorage<method> methodStorage;
	};

	static methodDBData* s_Data = new methodDBData();

	methodDefinition methodDB::createMethodDefinition(const char* p_className, const char* p_methodName, const char* const** p_args, uint32_t p_argcount)
	{
		methodDefinition methodDef;
		methodDef.id = std::hash<std::string>{}(std::string(p_className) + "::" + std::string(p_methodName));
		methodDef.m_class = p_className;
		methodDef.m_name = p_methodName;
		if (p_args && p_argcount > 0) 
		{
			for (uint32_t i = 0; i < p_argcount; ++i) 
				methodDef.args.push_back(std::string(*p_args[i])); // Add argument names to the vector
		}
		return methodDef;
	}


	/* internall functions*/
}
