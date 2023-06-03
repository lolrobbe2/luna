#include <lnpch.h>
#pragma warning (push,0)

#include <core/debug/log.h>

#include <spdlog/sinks/stdout_color_sinks.h>
#include <spdlog/sinks/basic_file_sink.h>
#pragma warning(pop)
#include <core/scripting/scriptingEngine.h>
#include <core/object/methodDB.h>
namespace luna {

	std::shared_ptr<spdlog::logger> Log::s_CoreLogger;
	std::shared_ptr<spdlog::logger> Log::s_ClientLogger;

	void Log::Init()
	{
		std::vector<spdlog::sink_ptr> logSinks;
		logSinks.emplace_back(std::make_shared<spdlog::sinks::stdout_color_sink_mt>());
		logSinks.emplace_back(std::make_shared<spdlog::sinks::basic_file_sink_mt>("luna.log", true));

		logSinks[0]->set_pattern("%^[%T] %n: %v%$");
		logSinks[1]->set_pattern("[%T] [%l] %n: %v");

		s_CoreLogger = std::make_shared<spdlog::logger>("LUNA", begin(logSinks), end(logSinks));
		spdlog::register_logger(s_CoreLogger);
		s_CoreLogger->set_level(spdlog::level::trace);
		s_CoreLogger->flush_on(spdlog::level::trace);

		s_ClientLogger = std::make_shared<spdlog::logger>("APP", begin(logSinks), end(logSinks));
		spdlog::register_logger(s_ClientLogger);
		s_ClientLogger->set_level(spdlog::level::trace);
		s_ClientLogger->flush_on(spdlog::level::trace);
	}

	void Log::Reinit()
	{
		spdlog::drop_all();
		std::vector<spdlog::sink_ptr> logSinks;
		logSinks.emplace_back(std::make_shared<spdlog::sinks::stdout_color_sink_mt>());
		logSinks.emplace_back(std::make_shared<spdlog::sinks::basic_file_sink_mt>("luna.log", true));

		logSinks[0]->set_pattern("%^[%T] %n: %v%$");
		logSinks[1]->set_pattern("[%T] [%l] %n: %v");

		s_CoreLogger = std::make_shared<spdlog::logger>("LUNA", begin(logSinks), end(logSinks));
		spdlog::register_logger(s_CoreLogger);
		s_CoreLogger->set_level(spdlog::level::trace);
		s_CoreLogger->flush_on(spdlog::level::trace);

		s_ClientLogger = std::make_shared<spdlog::logger>("APP", begin(logSinks), end(logSinks));
		spdlog::register_logger(s_ClientLogger);
		s_ClientLogger->set_level(spdlog::level::trace);
		s_ClientLogger->flush_on(spdlog::level::trace);
	}

	void Log::RegisterMethods()
	{
		LN_ADD_INTERNAL_CALL(Log, TraceInternal);
		LN_ADD_INTERNAL_CALL(Log, InfoInternal);
		LN_ADD_INTERNAL_CALL(Log, WarnInternal);
		LN_ADD_INTERNAL_CALL(Log, ErrorInternal);
		LN_ADD_INTERNAL_CALL(Log, CriticalInternal)
	}

	_ALWAYS_INLINE_ void Log::TraceInternal(MonoString* string)
	{
		LN_TRACE(std::string(mono_string_to_utf8(string)));
	}

	_ALWAYS_INLINE_ void Log::InfoInternal(MonoString* string)
	{
		LN_INFO(std::string(mono_string_to_utf8(string)));
	}
	_ALWAYS_INLINE_ void Log::WarnInternal(MonoString* string)
	{
		LN_WARN(std::string(mono_string_to_utf8(string)));
	}
	_ALWAYS_INLINE_ void Log::ErrorInternal(MonoString* string)
	{
		LN_ERROR(std::string(mono_string_to_utf8(string)));
	}
	_ALWAYS_INLINE_ void Log::CriticalInternal(MonoString* string)
	{
		LN_CRITICAL(std::string(mono_string_to_utf8(string)));
	}


}
