#include "monoType.h"
#include <mono/jit/jit.h>
#include <mono/metadata/assembly.h>
#include <mono/metadata/attrdefs.h>
#include <mono/metadata/object.h>

#include <core/scripting/monoClass.h>
namespace luna 
{
	namespace scripting 
	{
		monoType::monoType(MonoType* p_type) : p_type(p_type)
		{
			m_name = mono_type_get_name(p_type);
			m_size = mono_type_size(p_type, &m_alignement);
		}
		std::string monoType::getName()
		{
			return m_name;
		}
		int monoType::getSize()
		{
			return m_size;
		}
		int monoType::getAlignement()
		{
			return m_alignement;
		}
		MonoTypeEnum monoType::getType()
		{
			return (MonoTypeEnum)mono_type_get_type(p_type);
		}
		monoClass monoType::getClass()
		{
			return monoClass(mono_type_get_class(p_type));
		}
	}
}