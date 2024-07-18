#include "monoField.h"

#include <mono/jit/jit.h>
#include <mono/metadata/assembly.h>
#include <mono/metadata/attrdefs.h>
#include <mono/metadata/object.h>

#include "monoClass.h"
#include "monoType.h"
namespace luna 
{
	namespace scripting
	{
		monoField::monoField(MonoClass* p_class, MonoClassField* p_monoClassField) : p_class(p_class), p_monoClassField(p_monoClassField)
		{
			
		}
		const std::string& monoField::getName()
		{
			return mono_field_get_name(p_monoClassField);
		}
		monoClass monoField::getParentClass()
		{
			return monoClass(p_class);
		}
		monoType monoField::getType()
		{
			return monoType(mono_field_get_type(p_monoClassField));
		}
	}
}