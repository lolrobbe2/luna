#include "methodDB.h"
#include <core/scripting/scriptingEngine.h>
#define LN_ADD_INTERNAL_CALL(Name) mono_add_internal_call("Hazel.InternalCalls::" #Name, Name)

namespace luna
{
	void methodDB::setName(uuid nodeId, MonoString* name)
	{
		Node node = { nodeId,scripting::scriptingEngine::getContext() };
		node.setName(mono_string_to_utf8(name));
	}
}
