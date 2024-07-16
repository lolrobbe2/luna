#pragma once
#ifndef _MONO_CLASS_
#define _MONO_CLASS_
#include <core/core.h>

namespace luna 
{
	namespace scripting 
	{
		class monoMethod;
		class LN_API monoClass
		{
		public:
			monoClass() = default;
			monoClass(MonoClass* p_class);

			_ALWAYS_INLINE_ operator const std::string() const { return m_name; }
			_ALWAYS_INLINE_ const std::string getName() const { return m_name; } 
			_ALWAYS_INLINE_ const std::vector<monoMethod> getMethods() const;
			_ALWAYS_INLINE_ const std::vector<monoMethod> getMethodsAttribute(const std::string& attributeName);
			_ALWAYS_INLINE_ const monoMethod getMethod(const std::string& name,int paramCount) const;
		private:
			std::string m_name;
			MonoClass* p_class;
		};
	}
}

#endif // !_MONO_CLASS_

