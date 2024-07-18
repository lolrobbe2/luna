#include "monoArray.h"
#include <mono/jit/jit.h>
#include <mono/metadata/assembly.h>
#include <mono/metadata/attrdefs.h>
#include <mono/metadata/object.h>
namespace luna 
{
	namespace scripting 
	{
		monoArray::monoArray(monoClass monoClass, size_t size)
		{
			p_array = mono_array_new(mono_get_root_domain(), monoClass.getNative(), size);
		}
	}
}