#pragma once
#include <core/core.h>
#include <optional>
namespace luna
{
	struct signal
	{
		std::string signalName;
		uint8_t paramCount;
		MonoMethod* signalMethod;
	};

	class Signal
	{
	public:
		static void RegisterMethods();
	private:
		static void SignalEmitSignal(uint64_t emitterObjectId, std::string signal, void** args);
		static void SignalEmitGlobalSignal(uint64_t emitterObjectId, std::string signal, void** args);
	};

	class LN_API signalDB
	{
	public:
		static void registerSignal(signal& signal, std::string& className);
		static void deregisterSignal(std::string& signalName, std::string& className);
		static std::optional<std::vector<std::string>> getSignalNames(std::string& className);
		template<typename T> 
		static std::optional<std::vector<std::string>> getSignalNames();
	};
	template<typename T>
	inline std::optional<std::vector<std::string>> signalDB::getSignalNames()
	{
		return getSignalNames(stringify(T));
	}
}

