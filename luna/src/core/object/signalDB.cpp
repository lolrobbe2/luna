#include "signalDB.h"
#include <core/utils/objectStorage.h>
#include <core/scripting/scriptingEngine.h>
namespace luna 
{

	void signalDB::emitSignal(uint32_t objectId, std::string& functionName, void** params)
	{
		Node node{ objectId,scripting::scriptingEngine::getContext() };
	}
}