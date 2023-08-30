#include "signal.h"

#include <core/object/methodDB.h>
#include <core/object/objectDB.h>
#include <core/scripting/scriptingEngine.h>

namespace luna
{
#pragma region signal
	void Signal::RegisterMethods()
	{
		LN_ADD_INTERNAL_CALL(Signal, SignalEmitSignal);
		LN_ADD_INTERNAL_CALL(Signal, SignalEmitGlobalSignal);
	}
	void Signal::SignalEmitSignal(uint64_t emitterObjectId, std::string signal, void** args)
	{
		object((entt::entity)emitterObjectId, scripting::scriptingEngine::getContext()).emitSignal(signal.c_str(), args);
	}
	void Signal::SignalEmitGlobalSignal(uint64_t emitterObjectId, std::string signal, void** args)
	{
	}
#pragma endregion

#pragma region signalDB
	static std::unordered_map<std::string, std::vector<signal>> registeredSignals;

	void signalDB::registerSignal(signal& _signal, std::string& className)
	{
		auto& signalsIterator = registeredSignals.find(className);
		if(signalsIterator != registeredSignals.end())
		{
			auto& res = std::find_if(signalsIterator->second.begin(), signalsIterator->second.end(), [&](signal m_signal) {
				return m_signal.signalName == _signal.signalName;
				});
			if(res == signalsIterator->second.end()) return signalsIterator->second.push_back(_signal); //signal not registered
			res->paramCount = _signal.paramCount;
			res->signalMethod = _signal.signalMethod;
			res->signalName = _signal.signalName;
			return LN_CORE_WARN("signal has already been registered, and was overwritten! name: {0}, param count: {1}",_signal.signalName,_signal.paramCount); 
		}
		registeredSignals.insert({ className,{_signal} });
	}
	void signalDB::deregisterSignal(std::string& signalName, std::string& className)
	{
		auto& signalsIterator = registeredSignals.find(className);
		if (signalsIterator != registeredSignals.end())
		{
			auto& res = std::find_if(signalsIterator->second.begin(), signalsIterator->second.end(), [&](signal m_signal) {
				return m_signal.signalName == signalName;
				});
			if(res !=  signalsIterator->second.end()) signalsIterator->second.erase(res);
		}
	}
	std::vector<std::string> signalDB::getSignalNames(std::string& className)
	{
		std::string newClassName = camelToPascal(className);
		auto it = registeredSignals.find(newClassName);
		if (it != registeredSignals.end()) {
			const auto& [_, signals] = *it;

			std::vector<std::string> signalNames;
			signalNames.reserve(signals.size());
			for (const auto& [signalName, paramCount, signalMethod] : signals) {
				signalNames.push_back(signalName);
			}
			return signalNames;
		}

		return std::vector<std::string>();
	}
	const signal& signalDB::getSignalByName(std::string& className,const std::string& signalName)
	{
		std::string newClassName = camelToPascal(className);
		auto it = registeredSignals.find(newClassName);
		if (it != registeredSignals.end()) //class /type found
		{
			auto signalIt = std::find_if(it->second.begin(), it->second.end(), [&](signal m_signal) {
				return m_signal.signalName == signalName;
				});
			if (signalIt != it->second.end()) return *signalIt;
			
			LN_CORE_ERROR("could not find registered signal inside class, check all registered signals! {0}",signalName);
			return signal();
		}

		LN_CORE_ERROR("could not find class in signalDB! {0}",className);
		return signal();
	}
#pragma endregion signalDB
}


