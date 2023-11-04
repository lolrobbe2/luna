#pragma once
#include <lnpch.h>
#include <core/utils/json.h>
#include <core/debug/debugMacros.h>
#define JSON_OP(className) operator className()\
{\
	try\
	{\
		return jsonData.get<className>();\
	}\
	catch (const std::exception& err)\
	{\
		LN_CORE_ERROR("[JSON] error occured during conversion to" + std::string(#className) + ", error msg: {0}", err.what());\
		return className();\
	}\
}
namespace luna 
{
	namespace utils 
	{
		class json
		{
		public:
			json() = default;
			json(const std::filesystem::path& path);
			json(const std::string& jsonString);
			~json();    
			json operator [] (const std::string& key) { return json(jsonData[key]); }
			operator nlohmann::json() { return jsonData; }
			JSON_OP(std::string)
			JSON_OP(uint8_t)
			JSON_OP(uint16_t)
			JSON_OP(uint32_t)
			JSON_OP(uint64_t)
			nlohmann::json jsonData;
		private:
			json(const nlohmann::json& obj);
		
		};
	}
}