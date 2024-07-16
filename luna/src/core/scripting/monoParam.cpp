#include "monoParam.h"
#include <mono/metadata/loader.h>

#include <core/scripting/monoMethod.h>
namespace luna 
{
	namespace scripting 
	{
		monoParam::monoParam(MonoMethod* p_method,MonoType* p_type, int paramNumber,const char* p_name) : p_method(p_method),m_paramNumber(paramNumber)
		{
			m_type = p_type;
			m_name = p_name;
		}
		monoMethod monoParam::getMethod()
		{
			return monoMethod(p_method);
		}
	}
}

