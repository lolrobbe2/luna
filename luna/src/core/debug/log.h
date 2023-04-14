#pragma once

#define GLM_ENABLE_EXPERIMENTAL

// This ignores all warnings raised inside External headers
#pragma warning (push, 0)
#include <glm/gtx/string_cast.hpp>
#include <spdlog/spdlog.h>
#include <spdlog/fmt/ostr.h>
#pragma warning(pop)
#include <core/platform/platform.h>

extern "C" {
	typedef struct _MonoClass MonoClass;
	typedef struct _MonoObject MonoObject;
	typedef struct _MonoMethod MonoMethod;
	typedef struct _MonoAssembly MonoAssembly;
	typedef struct _MonoImage MonoImage;
	typedef struct _MonoClassField MonoClassField;
	typedef struct _MonoString MonoString;
}
namespace luna {

	class LN_API Log
	{
	public:
		static void Init();
		static void RegisterMethods();
		static std::shared_ptr<spdlog::logger> GetCoreLogger() { return s_CoreLogger; }
		static std::shared_ptr<spdlog::logger> GetClientLogger(){ return s_ClientLogger; }
		
		
		
	private:
		static void TraceInternal(MonoString* string);
		static void InfoInternal(MonoString* string);
		static void WarnInternal(MonoString* string);
		static void ErrorInternal(MonoString* string);
		static void CriticalInternal(MonoString* string);

		static std::shared_ptr<spdlog::logger> s_CoreLogger;
		static std::shared_ptr<spdlog::logger> s_ClientLogger;
	};



}

template<typename OStream, glm::length_t L, typename T, glm::qualifier Q>
inline OStream& operator<<(OStream& os, const glm::vec<L, T, Q>& vector)
{
	return os << glm::to_string(vector);
}

template<typename OStream, glm::length_t C, glm::length_t R, typename T, glm::qualifier Q>
inline OStream& operator<<(OStream& os, const glm::mat<C, R, T, Q>& matrix)
{
	return os << glm::to_string(matrix);
}

template<typename OStream, typename T, glm::qualifier Q>
inline OStream& operator<<(OStream& os, glm::qua<T, Q> quaternion)
{
	return os << glm::to_string(quaternion);
}

// core log macros
#define LN_CORE_TRACE(...)    ::luna::Log::GetCoreLogger()->trace(__VA_ARGS__)
#define LN_CORE_INFO(...)     ::luna::Log::GetCoreLogger()->info(__VA_ARGS__)
#define LN_CORE_WARN(...)     ::luna::Log::GetCoreLogger()->warn(__VA_ARGS__)
#define LN_CORE_ERROR(...)    ::luna::Log::GetCoreLogger()->error(__VA_ARGS__)
#define LN_CORE_CRITICAL(...) ::luna::Log::GetCoreLogger()->critical(__VA_ARGS__)

// client log macros
#define LN_TRACE(...)         ::luna::Log::GetClientLogger()->trace(__VA_ARGS__)
#define LN_INFO(...)          ::luna::Log::GetClientLogger()->info(__VA_ARGS__)
#define LN_WARN(...)          ::luna::Log::GetClientLogger()->warn(__VA_ARGS__)
#define LN_ERROR(...)         ::luna::Log::GetClientLogger()->error(__VA_ARGS__)
#define LN_CRITICAL(...)      ::luna::Log::GetClientLogger()->critical(__VA_ARGS__)