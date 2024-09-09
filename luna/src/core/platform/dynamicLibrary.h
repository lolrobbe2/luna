#pragma once
#ifndef _DYNAMIC_LIBRARY_
#define _DYNAMIC_LIBRARY_
#include <core/platform/platform.h>
#include <core/core.h>
#ifdef LN_PLATFORM_WINDOWS
typedef HMODULE dyLibHandle;
typedef FARPROC procAddress;
#define GET_PROC_ADDRESS(procName) GetProcAddress(m_handle,procName)
#define LOAD_DYLIB(libPath) LoadLibrary(libPath)
#define CLOSE_DYLIB(handle) FreeLibrary(handle)
#elif defined(LN_PLATFORM_LINUX) 
typedef void* dyLibHandle;
typedef void* procAddress
#define GET_PROC_ADDRESS(procName) dlsym(m_handle,procName)
#define LOAD_DYLIB(libPath) dlopen(libPath, RTLD_LAZY);
#define CLOSE_DYLIB(handle) dlclose(handle)
#endif
namespace luna
{
	namespace platform
	{
		class dynamicLibrary
		{
		public:
			dynamicLibrary(dyLibHandle handle);
			~dynamicLibrary();
			procAddress getProcAddress(const std::string& functionName);

		private:
			dyLibHandle m_handle;
		};
	}
}
#endif // !_DYNAMIC_LIBRARY_
