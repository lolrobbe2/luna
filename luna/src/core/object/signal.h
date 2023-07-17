#pragma once
#include <core/core.h>
namespace luna
{
	class Signal
	{
	public:
		static void RegisterMethods();
	private:
		static void SignalEmitSignal(uint64_t emitterObjectId, std::string signal, void** args);
		static void SignalEmitGlobalSignal(uint64_t emitterObjectId, std::string signal, void** args);
	};
}	

