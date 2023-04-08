#include "methodDB.h"
#include <core/scripting/scriptingEngine.h>
#ifndef TYPED_METHOD_BIND
class __UnexistingClass;
#define MB_T __UnexistingClass
#else
#define MB_T T
#endif



namespace luna
{
	#ifdef TYPED_METHOD_BIND
		template <class T, class... P>
	#else
		template <class... P>
	#endif
	struct Method
	{
		MonoMethod* externalMethod;
		void (MB_T::*method)(P...) = nullptr;
		int argsCount;
	};

	struct methodDBData
	{
		//TODO change map with hashed objStorage.
		std::map<std::string,Method<>> methodMap;
	};

	static methodDBData* s_Data = new methodDBData();

	void methodDB::init()
	{

	}

	void methodDB::bindObjectFunctions(const std::string& className) 
	{
		objectDB::classInfo* info = objectDB::getPtr(className);
		if (!info) return LN_CORE_ERROR("object class not registered {0}",className);
		Node* node = (Node*)info->creation_func();
		node->bindMethods();
	}


	void methodDB::bindFunctions()
	{
		for (const std::string& objectName : scripting::scriptingEngine::getAppClassNames()) 
		{
			bindObjectFunctions(objectName);
		}
	}


	/* internall functions*/
}
