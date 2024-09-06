#pragma once
#ifndef _MONO_TYPE_
#define _MONO_TYPE_
#include <core/core.h>
#include <mono/metadata/metadata.h>

namespace luna 
{
	namespace scripting 
	{
		class monoClass;
		class monoType
		{
		public:
			monoType() = default;
			monoType(MonoType* p_type);
			_ALWAYS_INLINE_ std::string getName();
			_ALWAYS_INLINE_ int getSize();
			_ALWAYS_INLINE_ int getAlignement();
			_ALWAYS_INLINE_ MonoTypeEnum getType();
			_ALWAYS_INLINE_ monoClass getClass();

		private:
			std::string m_name;
			int m_size,m_alignement;
			MonoType* p_type;
		};
	}
}
#endif // !_MONO_TYPE_


