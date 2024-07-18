#include "monoObject.h"
#include <mono/jit/jit.h>
#include <mono/metadata/assembly.h>
#include <mono/metadata/attrdefs.h>
#include <mono/metadata/object.h>
namespace luna 
{
	namespace scripting 
	{
		monoObject::monoObject(monoClass monoClass) : monoClass(monoClass)
		{
			p_object = mono_object_new_fast(getVTable());
			mono_runtime_object_init(p_object);
		}
		MonoObject* monoObject::getNative()
		{
			return p_object;
		}
		ref<monoObject> monoObject::clone()
		{
			return createRef<monoObject>(mono_object_clone(p_object),*this);
		}
		monoObject::monoObject(MonoObject* p_object, monoClass monoClass) : monoClass(monoClass), p_object(p_object)
		{
			
		}
	}
}

