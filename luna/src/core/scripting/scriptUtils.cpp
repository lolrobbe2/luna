#include "scriptUtils.h"
#include <core/scripting/scriptingEngine.h>
namespace luna
{
	namespace utils
	{
		std::vector<const char*> scriptUtils::getAppClassNames()
		{
			return scripting::scriptingEngine::getAppClassNames();
		}
	}
}
