#pragma once
#if defined(_WIN32) || defined(_WIN64)
	#define LN_PLATFORM_WINDOWS
	#define LN_PLATFORM_NAME "windows"
#elif defined (macintosh) || defined(Macintosh) || defined(__APPLE__) ||defined(__MACH__)
	#define LN_PLATFORM_MAC
	#define LN_PLATFORM_NAME "macintosh"
#elif defined(__linux__ )|| defined(linux) ||defined( __linux)
	#define LN_PLATFORM_LINUX
	#define LN_PLATFORM_NAME "linux"
#endif // defined(_WIN32 || _WIN64)








#ifdef LN_PLATFORM_WINDOWS
//#include<Windows.h>
#define GLFW_EXPOSE_NATIVE_WIN32
	#ifdef LN_BUILD_DLL
		#define LN_API __declspec(dllexport)
	#else
		#define LN_API __declspec(dllimport)
	#endif 
#define YAML_CPP_API LN_API
#elif defined(LN_PLATFORM_LINUX)
	#error luna does not support linux systems!
#elif defined(LN_PLATFORM_MAC)
	#error luna does not support macintosh systems at the moment support coming soon!
#else
	#error luna only supports windows!
#endif 


namespace luna
{
	class Time
	{
	public:
		static float getTime();

	};


}



