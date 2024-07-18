#pragma once
#ifndef _MONO_ARRAY_
#define _MONO_ARRAY_

#include "monoClass.h"

namespace luna
{
	namespace scripting
	{
		class monoArray
		{
		public:
			monoArray(monoClass monoClass,size_t size);
		private:
			MonoArray* p_array;
		};
	}
}
#endif // !_MONO_ARRAY_