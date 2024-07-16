#pragma once
#ifndef _MONO_METHOD_
#define _MONO_METHOD_
#include <core/core.h>
#include <mono/metadata/metadata.h>
namespace luna 
{
	namespace scripting 
	{
		class monoParam;
		class monoClass;
		class LN_API monoMethod
		{
		public:
			monoMethod(MonoMethod* p_method) : p_method(p_method) {};
			_ALWAYS_INLINE_ std::string getName() const;
			_ALWAYS_INLINE_ MonoMethodSignature* getSingature() const;

			_ALWAYS_INLINE_ uint32_t getParamCount() const;
			_ALWAYS_INLINE_ std::vector<std::string> getParamNames() const;
			_ALWAYS_INLINE_ std::vector<monoParam> getParams() const;

			_ALWAYS_INLINE_ std::vector<monoClass> getAttributes() const;
			_ALWAYS_INLINE_ bool hasAttribute(const std::string& name) const;

			_ALWAYS_INLINE_ monoClass getAttribute(const std::string& name) const;
			_ALWAYS_INLINE_ MonoMethod* getNative() const { return p_method;}
		private:
			MonoMethod* p_method;
		};
	}
}
#endif // !_MONO_METHOD_


