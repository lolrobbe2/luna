#pragma once
#ifndef CORE
#define CORE
#define BIT(x) (1<<x)

#define LN_BIND_EVENT_FN(fn) [this](auto&&... args) -> decltype(auto) { return this->fn(std::forward<decltype(args)>(args)...); }
#pragma warning(push, 0)
#include <memory>
#include <core/debug/log.h>
#define LN_PROFILE 1
#include <core/debug/instrumentor.h>
#pragma warning(pop)
#ifdef _DEBUG
	#define ENABLE_VALIDATION_LAYERS
#endif // _DEBUG

#define MAJOR 0
#define MINOR 0 
#define PATCH 2 

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

