#pragma once
//from hazel2D engine
#include <core/debug/log.h>

#include <algorithm>
#include <chrono>
#include <fstream>
#include <iomanip>
#include <string>
#include <thread>
#include <mutex>
#include <sstream>
#include <filesystem>

namespace luna {

	using floatingPointMicroseconds = std::chrono::duration<double, std::micro>;

	struct profileResult
	{
		std::string Name;

		floatingPointMicroseconds Start;
		std::chrono::microseconds ElapsedTime;
		std::thread::id ThreadID;
	};

	struct instrumentationSession
	{
		std::string Name;
	};

	class instrumentor
	{
	public:
		instrumentor(const instrumentor&) = delete;
		instrumentor(instrumentor&&) = delete;

		void BeginSession(const std::string& name, const std::string& filepath = "results.json")
		{
			std::lock_guard lock(m_Mutex);
			if (m_CurrentSession)
			{
				// If there is already a current session, then close it before beginning new one.
				// Subsequent profiling output meant for the original session will end up in the
				// newly opened session instead.  That's better than having badly formatted
				// profiling output.
				if (Log::GetCoreLogger()) // Edge case: BeginSession() might be before Log::Init()
				{
					LN_CORE_ERROR("instrumentor::BeginSession('{0}') when session '{1}' already open.", name, m_CurrentSession->Name);
				}
				InternalEndSession();
			}
			m_OutputStream.open(filepath);

			if (m_OutputStream.is_open() && m_OutputStream.good())
			{
				m_CurrentSession = new instrumentationSession({ name });
				WriteHeader();
			}
			else
			{
				
				if (Log::GetCoreLogger()) LN_CORE_ERROR("instrumentor could not open results file '{0}'.", filepath);
				std::filesystem::path _path(filepath);
				std::string directory = _path.parent_path().string();
				if (Log::GetCoreLogger()) LN_CORE_ERROR("creating dir {0}", directory);
				mkdir(directory.c_str());
			}
		}

		void EndSession()
		{
			std::lock_guard lock(m_Mutex);
			InternalEndSession();
		}

		void WriteProfile(const profileResult& result)
		{
			std::stringstream json;

			json << std::setprecision(3) << std::fixed;
			json << ",{";
			json << "\"cat\":\"function\",";
			json << "\"dur\":" << (result.ElapsedTime.count()) << ',';
			json << "\"name\":\"" << result.Name << "\",";
			json << "\"ph\":\"X\",";
			json << "\"pid\":0,";
			json << "\"tid\":" << result.ThreadID << ",";
			json << "\"ts\":" << result.Start.count();
			json << "}";

			std::lock_guard lock(m_Mutex);
			if (m_CurrentSession)
			{
				m_OutputStream << json.str();
				m_OutputStream.flush();
			}
		}

		static instrumentor& Get()
		{
			static instrumentor instance;
			return instance;
		}
	private:
		instrumentor()
			: m_CurrentSession(nullptr)
		{
		}

		~instrumentor()
		{
			EndSession();
		}

		void WriteHeader()
		{
			m_OutputStream << "{\"otherData\": {},\"traceEvents\":[{}";
			m_OutputStream.flush();
		}

		void WriteFooter()
		{
			m_OutputStream << "]}";
			m_OutputStream.flush();
		}

		// Note: you must already own lock on m_Mutex before
		// calling InternalEndSession()
		void InternalEndSession()
		{
			if (m_CurrentSession)
			{
				WriteFooter();
				m_OutputStream.close();
				delete m_CurrentSession;
				m_CurrentSession = nullptr;
			}
		}
	private:
		std::mutex m_Mutex;
		instrumentationSession* m_CurrentSession;
		std::ofstream m_OutputStream;
	};

	class instrumentationTimer
	{
	public:
		instrumentationTimer(const char* name)
			: m_Name(name), m_Stopped(false)
		{
			m_StartTimepoint = std::chrono::steady_clock::now();
		}

		~instrumentationTimer()
		{
			if (!m_Stopped)
				Stop();
		}

		void Stop()
		{
			auto endTimepoint = std::chrono::steady_clock::now();
			auto highResStart = floatingPointMicroseconds{ m_StartTimepoint.time_since_epoch() };
			auto elapsedTime = std::chrono::time_point_cast<std::chrono::microseconds>(endTimepoint).time_since_epoch() - std::chrono::time_point_cast<std::chrono::microseconds>(m_StartTimepoint).time_since_epoch();

			instrumentor::Get().WriteProfile({ m_Name, highResStart, elapsedTime, std::this_thread::get_id() });

			m_Stopped = true;
		}
	private:
		const char* m_Name;
		std::chrono::time_point<std::chrono::steady_clock> m_StartTimepoint;
		bool m_Stopped;
	};

	namespace instrumentorUtils {

		template <size_t N>
		struct ChangeResult
		{
			char Data[N];
		};

		template <size_t N, size_t K>
		constexpr auto CleanupOutputString(const char(&expr)[N], const char(&remove)[K])
		{
			ChangeResult<N> result = {};

			size_t srcIndex = 0;
			size_t dstIndex = 0;
			while (srcIndex < N)
			{
				size_t matchIndex = 0;
				while (matchIndex < K - 1 && srcIndex + matchIndex < N - 1 && expr[srcIndex + matchIndex] == remove[matchIndex])
					matchIndex++;
				if (matchIndex == K - 1)
					srcIndex += matchIndex;
				result.Data[dstIndex++] = expr[srcIndex] == '"' ? '\'' : expr[srcIndex];
				srcIndex++;
			}
			return result;
		}
	}
}
//#undef LN_PROFILE
#ifndef LN_PROFILE
	#define LN_PROFILE 0
#endif // !LN_PROFILE

#if LN_PROFILE
// Resolve which function signature macro will be used. Note that this only
// is resolved when the (pre)compiler starts, so the syntax highlighting
// could mark the wrong one in your editor!
#if defined(__GNUC__) || (defined(__MWERKS__) && (__MWERKS__ >= 0x3000)) || (defined(__ICC) && (__ICC >= 600)) || defined(__ghs__)
#define LN_FUNC_SIG __PRETTY_FUNCTION__
#elif defined(__DMC__) && (__DMC__ >= 0x810)
#define LN_FUNC_SIG __PRETTY_FUNCTION__
#elif (defined(__FUNCSIG__) || (_MSC_VER))
#define LN_FUNC_SIG __FUNCSIG__
#elif (defined(__INTEL_COMPILER) && (__INTEL_COMPILER >= 600)) || (defined(__IBMCPP__) && (__IBMCPP__ >= 500))
#define LN_FUNC_SIG __FUNCTION__
#elif defined(__BORLANDC__) && (__BORLANDC__ >= 0x550)
#define LN_FUNC_SIG __FUNC__
#elif defined(__STDC_VERSION__) && (__STDC_VERSION__ >= 199901)
#define LN_FUNC_SIG __func__
#elif defined(__cplusplus) && (__cplusplus >= 201103)
#define LN_FUNC_SIG __func__
#else
#define LN_FUNC_SIG "LN_FUNC_SIG unknown!"
#endif

#define LN_PROFILE_BEGIN_SESSION(name, filepath) ::luna::instrumentor::Get().BeginSession(name, filepath)
#define LN_PROFILE_END_SESSION() ::luna::instrumentor::Get().EndSession()
#define LN_PROFILE_SCOPE_LINE2(name, line) constexpr auto fixedName##line = ::luna::instrumentorUtils::CleanupOutputString(name, "__cdecl ");\
											   ::luna::instrumentationTimer timer##line(fixedName##line.Data)
#define LN_PROFILE_SCOPE_LINE(name, line) LN_PROFILE_SCOPE_LINE2(name, line)
#define LN_PROFILE_SCOPE(name) LN_PROFILE_SCOPE_LINE(name, __LINE__)
#define LN_PROFILE_FUNCTION() LN_PROFILE_SCOPE(LN_FUNC_SIG)
#else
#define LN_PROFILE_BEGIN_SESSION(name, filepath)
#define LN_PROFILE_END_SESSION()
#define LN_PROFILE_SCOPE(name)
#define LN_PROFILE_FUNCTION()
#endif