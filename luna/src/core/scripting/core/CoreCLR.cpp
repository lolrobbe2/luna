#include "CoreCLR.h"
#include <core/debug/debugMacros.h>
#include <core/platform/platformUtils.h>
namespace luna
{
	namespace scripting
	{
		void CoreCLR::init()
		{
			std::filesystem::path coreCLRPath = getCoreCLRPath();
			coreCLRLib = platform::os::loadDynamicLib(coreCLRPath);
		}
		std::string CoreCLR::getCoreCLRPath()
		{
			const char* envPath = getenv("CORECLR_PATH");
			LN_ERR_FAIL_NULL_V_MSG(envPath,"", "[CoreCLR] could not resolve CoreCLR lib Path!");
			return envPath;
		}
	}
}