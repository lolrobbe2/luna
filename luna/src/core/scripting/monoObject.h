#pragma once
#ifndef _MONO_OBJECT_
#define _MONO_OBJECT_

#include "monoClass.h"
namespace luna 
{
	namespace scripting
	{
		class monoObject : public monoClass
		{
		public:
			monoObject() = default;
			monoObject(monoClass monoClass);
			monoObject(MonoObject* p_object);
			monoObject invoke(monoMethod method, void** params);
		    MonoObject* getNative();
			_ALWAYS_INLINE_ monoObject clone();
		private:
			monoObject(MonoObject* p_object, monoClass monoClass);
		private:
			MonoObject* p_object;
		};
	}
}

#endif // !_MONO_OBJECT_