#include "methodDB.h"
#include <core/scripting/scriptingEngine.h>
#define LN_ADD_INTERNAL_CALL(Class,Function) luna::methodDB::bindInternalFunction<Class>(#Function,Function);
#define LN_ADD_EXTERNAL_CALL(Class,Function) //TODO

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
		void (MB_T::*method)(P...) = nullptr;
		int argsCount;
	};

	struct methodDBData
	{
		//TODO change map with hashed objStorage.
		std::map<std::string,Method<>> methodMap;
	};

	static s_Data = new methodDBData();

	void methodDB::init();

	void methodDB::bindFunctions()
	{
		LN_ADD_INTERNAL_CALL(Node, setName);
	}

	void methodDB::setName(uuid nodeId, MonoString* name)
	{
		Node node = { nodeId,scripting::scriptingEngine::getContext() };
		node.setName(mono_string_to_utf8(name));
	}
}
