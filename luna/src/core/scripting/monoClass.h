#pragma once
#ifndef _MONO_CLASS_
#define _MONO_CLASS_
#include <core/core.h>

struct MonoVTable;

namespace luna 
{
	namespace scripting 
	{
		class monoMethod;
		class monoObject; //AKA instance

		class LN_API monoClass
		{
		public:
			monoClass() = default;
			monoClass(MonoClass* p_class);

			_ALWAYS_INLINE_ operator const std::string() const { return m_name; }
			_ALWAYS_INLINE_ const std::string& getName() const { return m_name; } 
			_ALWAYS_INLINE_ const std::vector<monoMethod> getMethods() const;
			_ALWAYS_INLINE_ const std::vector<monoMethod> getMethodsAttribute(const std::string& attributeName);
			_ALWAYS_INLINE_ const monoMethod getMethod(const std::string& name,int paramCount) const;
			_ALWAYS_INLINE_ const monoClass getParent() const;
			_ALWAYS_INLINE_ ref<monoObject> instanciate();

			_ALWAYS_INLINE_ MonoClass* getNative() { return p_class; }

			_ALWAYS_INLINE_ operator bool() { return p_class && p_VTable; };
		protected:
			_ALWAYS_INLINE_ MonoVTable* getVTable() const { return p_VTable; }

		private:
			std::string m_name;
			MonoVTable* p_VTable; // Virtual function table (contains pointers to all functions, used for performance reasons!
			MonoClass* p_class;
		};
	}
}

#endif // !_MONO_CLASS_

