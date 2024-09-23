 #pragma once
#ifndef _CORE_
#define _CORE_
#include <lnpch.h>
#define BIT(x) (1<<x)

#define LN_BIND_EVENT_FN(fn) [this](auto&&... args) -> decltype(auto) { return this->fn(std::forward<decltype(args)>(args)...); }
#define LN_TYPE_TO_NAME(type) #type;
#ifndef LN_UNROLL_LOOP
	#if defined(_MSC_VER) // Microsoft Visual Studio
		#define LN_UNROLL_LOOP _Pragma("loop(hint_parallel(1))")
	#elif defined(__GNUC__) || defined(__clang__) // GCC or Clang
		#define LN_UNROLL_LOOP _Pragma("unroll")
	#elif defined(__INTEL_COMPILER) // Intel C++ Compiler
		#define LN_UNROLL_LOOP _Pragma("unroll")
	#else // Fallback for other compilers
		#define LN_UNROLL_LOOP // No unroll pragma
	#endif
#endif // !LN_UNROLL_LOOP


#ifndef LN_UNROLL_LOOP_COUNT
	#if _MSC_VER // Microsoft Visual Studio
		// MSVC does not directly support unroll factor, use hint_parallel
		#define LN_UNROLL_LOOP_COUNT(count) _Pragma("loop(hint_parallel(1))")
	#elif defined(__GNUC__) || defined(__clang__) // GCC or Clang
		// GCC and Clang support specifying unroll factor
		#define LN_UNROLL_LOOP_COUNT(count) _Pragma("unroll " #count)
	#elif defined(__INTEL_COMPILER) // Intel C++ Compiler
		// Intel C++ Compiler supports unroll factor
		#define LN_UNROLL_LOOP_COUNT(count) _Pragma("unroll " #count)
	#else // Fallback for other compilers
		#define LN_UNROLL_LOOP_COUNT(count) // No unroll pragma
	#endif
#endif // !LN_UNROLL_LOOP_COUNT

#pragma warning(push, 0)
#include <memory>
#include <core/debug/log.h>
#ifdef LN_DEBUG
	#define LN_PROFILE 1
#else
	#define LN_PROFILE 0
#endif
#include <core/debug/instrumentor.h>
#pragma warning(pop)
#ifdef _DEBUG
	#define ENABLE_VALIDATION_LAYERS
#endif // _DEBUG

#ifndef _ALWAYS_INLINE_
#if defined(__GNUC__)
#define _ALWAYS_INLINE_ __attribute__((always_inline)) inline
#elif defined(_MSC_VER)
#define _ALWAYS_INLINE_ __forceinline
#else
#define _ALWAYS_INLINE_ inline
#endif
#endif

#ifndef _NO_INLINE_
#define _NO_INLINE_ __declspec(noinline)
#endif

#ifndef stringify
#define stringify( name ) #name
#endif

#define MAJOR 0
#define MINOR 0 
#define PATCH 8
namespace luna
{
	template<typename T>
	using scope = std::unique_ptr<T>;
	template<typename T, typename ... Args>
	constexpr scope<T> createScope(Args&& ... args)
	{
		return std::make_unique<T>(std::forward<Args>(args)...);
	}

	template<typename T>
	using ref = std::shared_ptr<T>;
	template<typename T, typename ... Args>
	constexpr ref<T> createRef(Args&& ... args)
	{
		return std::make_shared<T>(std::forward<Args>(args)...);
	}
	/* start of c++ wizardry from GD */
#define memnew(m_class) _post_initialize(new m_class)
	_ALWAYS_INLINE_ static void postinitialize_handler(void*) {}

	template <class T>
	_ALWAYS_INLINE_ static T* _post_initialize(T* p_obj)
	{
		postinitialize_handler(p_obj);
		return p_obj;
	}
	/* end of c++ wizardry from GD*/
}
#endif // !CORE

