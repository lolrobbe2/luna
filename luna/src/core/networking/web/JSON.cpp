#include "JSON.h"
#include <core/debug/debugMacros.h>
namespace luna 
{
	namespace utils
	{
		json::json(std::filesystem::path path)
		{
			try 
			{
				jsonData = nlohmann::json::parse(std::ifstream(path.string()));
			}
			catch (const std::exception& err)
			{
				LN_CORE_ERROR("[JSON] error occured during file parsing, error msg: {0}", err.what());
			}
		}
		json::json(const nlohmann::json& obj)
		{
			jsonData = obj;
		}
	}
}

