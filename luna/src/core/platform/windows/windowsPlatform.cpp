#include <core/platform/platform.h>
#include <core/core.h>
#ifdef  LN_PLATFORM_WINDOWS
namespace luna
{
	float Time::getTime()
	{
		return glfwGetTime();
	}
}
#endif //  LN_PLATFORM_WINDOWS

