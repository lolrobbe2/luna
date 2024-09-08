#pragma once
#ifndef _CALLABLE_
#define _CALLABLE_
#include <core/object/object.h>
namespace luna 
{
	class LN_API object;
	struct methodDefinition;
	class callable
	{
	public:
		callable(object& object, const std::string& name);
		callable(object& object, const char* name);
	private:
		object m_object;
		methodDefinition m_methodDefinition;
	};


}
#endif // !_CALLABLE_
