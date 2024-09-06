#pragma once
#ifndef _MONO_FIELD_
#define _MONO_FIELD_

#include <core/core.h>

namespace luna 
{
	namespace scripting 
	{
		class monoClass;
		class monoType;
		class monoField
		{
		public:
			monoField(MonoClass* p_class,MonoClassField* p_monoClassField);
			_ALWAYS_INLINE_ const std::string& getName();
			_ALWAYS_INLINE_ monoClass getParentClass();
			_ALWAYS_INLINE_ monoType getType();
		private:
			MonoClass* p_class;
			MonoClassField* p_monoClassField;
		};
	}
}
#endif // !_MONO_FIELD_

