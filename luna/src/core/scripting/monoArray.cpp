#include "monoArray.h"
#include <core/debug/debugMacros.h>
#include <mono/jit/jit.h>
#include <mono/metadata/assembly.h>
#include <mono/metadata/attrdefs.h>
#include <mono/metadata/object.h>
#include <core/scripting/monoObject.h>
namespace luna 
{
	namespace scripting 
	{
		monoObject Iterator::operator*() const
		{
			return monoObject(mono_array_get(p_array, MonoObject*, index));
		}

		Iterator& Iterator::operator=(monoObject value)
		{
			mono_array_set(p_array, MonoObject*, index, value.getNative());
			return *this;
		}

		monoArray::monoArray(monoClass monoClass, size_t size)
		{
			p_array = mono_array_new(mono_get_root_domain(), monoClass.getNative(), size);
		}

		monoArray::monoArray(MonoArray* p_array) : p_array(p_array)
		{
		}

		size_t monoArray::size() const
		{
			return mono_array_length(p_array);
		}

		Iterator monoArray::operator[](int index)
		{
			LN_ERR_FAIL_COND_V_MSG(index < 0 || index >= size(), Iterator(p_array,0), "[Mono] index out of range! returned 0 index!");

			return Iterator(p_array, index);
		}
		
	}
}