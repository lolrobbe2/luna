#include "methodDB.h"
#include <core/scripting/scriptingEngine.h>
#define LN_ADD_INTERNAL_CALL(Class,Function) luna::methodDB::bindInternalFunction<Class>(#Function,Function);
#define LN_ADD_EXTERNAL_CALL(Class,Function) //TODO
namespace luna
{
	struct methodDBData
	{
		//TODO change map with hashed objStorage.
		
	};

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
