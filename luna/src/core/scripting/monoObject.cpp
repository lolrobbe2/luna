#include "monoObject.h"
#include <mono/jit/jit.h>
#include <mono/metadata/assembly.h>
#include <mono/metadata/attrdefs.h>
#include <mono/metadata/object.h>
#include <core/scripting/monoMethod.h>
namespace luna 
{
	namespace scripting 
	{
		monoObject::monoObject(monoClass monoClass) : monoClass(monoClass)
		{
			p_object = mono_object_new_fast(getVTable());
			mono_runtime_object_init(p_object);
		}
		monoObject::monoObject(MonoObject* p_object) : p_object(p_object),monoClass(p_object ? mono_object_get_class(p_object) : nullptr)
		{
			
		}
		monoObject monoObject::invoke(monoMethod method, void** params)
		{
			if (!method) return nullptr;
			return monoObject(mono_runtime_invoke(method.getNative(), p_object, params, nullptr));
		}
		MonoObject* monoObject::getNative()
		{
			return p_object;
		}
		monoObject monoObject::clone()
		{
			return monoObject(mono_object_clone(p_object), *this);
		}
		monoObject::monoObject(MonoObject* p_object, monoClass monoClass) : monoClass(monoClass), p_object(p_object)
		{
			
		}
	}
}

