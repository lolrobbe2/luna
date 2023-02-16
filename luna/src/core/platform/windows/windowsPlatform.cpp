#include "lnpch.h"
#include <core/platform/platform.h>
#ifdef  LN_PLATFORM_WINDOWS
namespace luna
{
	float Time::getTime()
	{
		return glfwGetTime();
	}
}
#endif //  LN_PLATFORM_WINDOWS

