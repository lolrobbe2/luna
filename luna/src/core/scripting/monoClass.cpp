#include "monoClass.h"

#include <mono/jit/jit.h>
#include <mono/metadata/assembly.h>
#include <mono/metadata/attrdefs.h>
#include <mono/metadata/object.h>
#include "monoMethod.h"
#include "monoObject.h"
namespace luna
{
	namespace scripting
	{
		monoClass::monoClass(MonoClass* p_class) : p_class(p_class), m_name(mono_class_get_name(p_class))
		{
			p_VTable = mono_class_vtable(mono_get_root_domain(), p_class);
		}
		const std::vector<monoMethod> monoClass::getMethods() const
		{
			void* iter = nullptr;
			MonoMethod* p_method;

			std::vector<monoMethod> methods;

			while ((p_method = mono_class_get_methods(p_class, &iter)) != nullptr)
				methods.push_back(p_method);

			return methods;
		}
		const std::vector<monoMethod> monoClass::getMethodsAttribute(const std::string& attributeName)
		{
			std::vector<monoMethod> methods;
			for(const monoMethod& method : getMethods())
				if(method.hasAttribute(attributeName)) methods.push_back(method);
			return methods;
		}
		const monoMethod monoClass::getMethod(const std::string& name, int paramCount) const
		{
			return mono_class_get_method_from_name(p_class,name.c_str(),paramCount);
		}

		ref<monoObject> monoClass::instanciate()
		{
			return createRef<monoObject>(*this);
		}
	}
}