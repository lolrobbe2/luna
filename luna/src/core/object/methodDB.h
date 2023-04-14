#pragma once
#define LN_ADD_INTERNAL_CALL(Class,Function) luna::methodDB::bindInternalFunction<Class>(#Function,Function);
#include <core/core.h>
#include <core/debug/uuid.h>
#include <mono/jit/jit.h>
#include <mono/metadata/assembly.h>
#include <mono/metadata/attrdefs.h>
namespace luna
{
	/**
	 * @brief holds all the internal and external functions + corresponding binding code.
	 */
	class methodDB
	{
	public:
		static void init();
		template<class T>
		static void bindInternalFunction(const std::string& functionName,const void* functionPtr);
		
	private:
		static void bindObjectFunctions(const std::string& className);
		static void bindFunctions();
	};


	template<class T>
	void methodDB::bindInternalFunction(const std::string& functionName,const void* functionPtr)
	{
		std::stringstream fullName(typeid(T).name());
		std::stringstream funcName(functionName);
		std::vector<std::string> classSeglist;
		std::vector<std::string> functionNameSeglist;
		std::string segment;
		while (std::getline(fullName, segment, ':')) classSeglist.push_back(segment);
		while (std::getline(funcName, segment, ':')) functionNameSeglist.push_back(segment);

		const std::string className = classSeglist.back();
		const std::string functionSignature = "Luna." + className + "::" + functionNameSeglist.back();
		mono_add_internal_call(functionSignature.c_str(),functionPtr);
	}
}
