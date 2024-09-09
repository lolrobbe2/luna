#include "dynamicLibrary.h"
#include <core/debug/debugMacros.h>
namespace luna
{
	namespace platform
	{
		dynamicLibrary::dynamicLibrary(dyLibHandle handle) : m_handle(handle) 
		{
			
		}
		dynamicLibrary::~dynamicLibrary()
		{
			CLOSE_DYLIB(m_handle);
		}
		procAddress dynamicLibrary::getProcAddress(const std::string& functionName)
		{
			procAddress proc = GET_PROC_ADDRESS(functionName.c_str());
			LN_ERR_FAIL_NULL_MSG(proc, "[dynamicLibrary] unable to load procedure!");
			return proc;
		}
	}
}
