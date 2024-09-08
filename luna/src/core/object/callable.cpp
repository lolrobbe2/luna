#include "callable.h"
namespace luna
{
	callable::callable(object& object, const std::string& name) : callable(object,name.c_str())
	{
	}
	callable::callable(object& object, const char* name) : m_object(object)
	{
	}
}

