#pragma once
#ifndef _CORE_CLR_
#define _CORE_CLR_
#include <core/core.h>
#include <core/thirdParty/coreclr_delegates.h>
#include <core/thirdParty/hostfxr.h>
#include <core/platform/dynamicLibrary.h>
namespace luna
{
	namespace scripting
	{
		class CoreCLR
		{
		public:
			void init();

		private:
			ref<platform::dynamicLibrary> coreCLRLib = nullptr;
			std::string getCoreCLRPath();
		};
	}
}
#endif // !_CORE_CLR_
