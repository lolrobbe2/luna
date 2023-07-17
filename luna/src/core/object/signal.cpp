#include "signal.h"

#include <core/object/methodDB.h>

namespace luna
{
	void Signal::RegisterMethods()
	{
		LN_ADD_INTERNAL_CALL(Signal, SignalEmitSignal);
		LN_ADD_INTERNAL_CALL(Signal, SignalEmitGlobalSignal);
	}
	void Signal::SignalEmitSignal(uint64_t emitterObjectId, std::string signal, void** args)
	{
	}
	void Signal::SignalEmitGlobalSignal(uint64_t emitterObjectId, std::string signal, void** args)
	{
	}
}


