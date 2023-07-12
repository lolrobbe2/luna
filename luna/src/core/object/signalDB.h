#pragma once
#include <core/core.h>
#define LUNA_REGISTER_SIGNAL(className,functionName,params)
#define LUNA_EMIT_SIGNAL(signalName,params) 
namespace luna 
{
	struct signal
	{
		signal(std::string& className, uint8_t params) : className(className), params(params) {}
		std::string className;
		uint8_t params;
	};
	class signalDB
	{
		/**
		* @brief registers a custom signal, returns true when succesful and false when unsuccesful
		*/
		_ALWAYS_INLINE_ static bool registerSignal(std::string& className,std::string& functionName,uint8_t paramsAmount) 
		{
			auto& signalVector = singalClassMap.find(className);
			if (signalVector != singalClassMap.end() && std::find(signalVector->second.begin(), signalVector->second.end(), signal(functionName, paramsAmount)) != signalVector->second.end())
			{
				signalVector->second.push_back(signal(functionName, paramsAmount));
				return true;
			} 
			return false;
		}
		_ALWAYS_INLINE_ static void emitSignal(uint32_t objectId, std::string& functionName, void** params);
	private:
		inline static std::unordered_map<std::string, std::vector<signal>> singalClassMap;
	};
}

