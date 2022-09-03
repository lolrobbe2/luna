#pragma once
#if defined(_WIN32) || defined(_WIN64)
	#define LN_PLATFORM_WINDOWS
#elif defined (macintosh) || defined(Macintosh) || defined(__APPLE__) ||defined(__MACH__)
	#define LN_PLATFORM_MAC
#elif defined(__linux__ )|| defined(linux) ||defined( __linux)
	#define LN_PLATFORM_LINUX
#endif // defined(_WIN32 || _WIN64)








#ifdef LN_PLATFORM_WINDOWS
#include<Windows.h>
	#ifdef LN_BUILD_DLL
		#define LN_API __declspec(dllexport)
	#else
		#define LN_API __declspec(dllimport)
	#endif 
#elif defined(LN_PLATFORM_LINUX)
	#error luna does not support linux systems!
#elif defined(LN_PLATFORM_MAC)
	#error luna does not support macintosh systems at the moment support coming soon!
#else
	#error luna only supports windows!
#endif 
