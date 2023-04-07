#pragma once
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
		static void bindFunctions();
		template<class T>
		static void bindInternalFunction(const std::string& functionName,const void* functionPtr);
		static void setName(uuid nodeId, MonoString* name);
	private:
		template <class T>
		_ALWAYS_INLINE_ static std::string getClassName();
	
	};


	template<class T>
	_ALWAYS_INLINE_ void methodDB::bindInternalFunction(const std::string& functionName,const void* functionPtr)
	{
		const std::string className = getCLassName<T>();
		const std::string functionSignature = "luna." + className + "::" + functionName;
		mono_add_internal_call(functionSignature.c_str(),functionPtr);
	}

	template<class T>
	_ALWAYS_INLINE_ std::string methodDB::getClassName()
	{
		std::stringstream fullName(typeid(T).name());
		std::vector<std::string> seglist;
		std::string segment;
		while (std::getline(fullName, segment, ':')) seglist.push_back(segment);
		return seglist.back();
	}
}
