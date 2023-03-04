#pragma once
#ifndef CORE
#define CORE
#define BIT(x) (1<<x)

#define LN_BIND_EVENT_FN(fn) [this](auto&&... args) -> decltype(auto) { return this->fn(std::forward<decltype(args)>(args)...); }
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


#define MAJOR 0
#define MINOR 0 
#define PATCH 5
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
}
#endif // !CORE

