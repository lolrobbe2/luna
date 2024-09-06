#include "monoMethod.h"
#include <mono/jit/jit.h>
#include <mono/metadata/assembly.h>
#include <mono/metadata/debug-helpers.h>
#include <mono/metadata/attrdefs.h>
#include <mono/metadata/object.h>

#include <core/scripting/monoParam.h>
#include <core/scripting/monoClass.h>
namespace luna
{
	namespace scripting 
	{
		std::string monoMethod::getName() const
		{
			return std::string(mono_method_get_name(p_method));
		}
		MonoMethodSignature* monoMethod::getSingature() const
		{
			return mono_method_signature(p_method);
		}
		uint32_t monoMethod::getParamCount() const
		{
			return mono_signature_get_param_count(getSingature());
		}
		std::vector<std::string> monoMethod::getParamNames() const
		{
			const char** p_names = (const char**)malloc(getParamCount() * sizeof(const char*));
			mono_method_get_param_names(p_method, p_names);

			std::vector<std::string> paramNames;

			for (size_t i = 0; i < getParamCount(); i++)
				paramNames.push_back(p_names[i]);

			return paramNames;
		}
		std::vector<monoParam> monoMethod::getParams() const
		{
			MonoType* paramType = nullptr;
			void* iter = nullptr;
			std::vector<monoParam> params;

			int i = 0;
			std::vector<std::string> paramNames = getParamNames();
			

			while ((paramType = mono_signature_get_params(getSingature(), &iter)) != nullptr)
				params.push_back(monoParam(p_method, paramType,i,paramNames[i].c_str())); i++;

			return params;
		}

		std::vector<monoClass> monoMethod::getAttributes() const
		{
			MonoCustomAttrInfo* attrs = mono_custom_attrs_from_method(p_method);

			if (!attrs) return std::vector<monoClass>();

			std::vector<monoClass> attributes;

			for (int i = 0; i < attrs->num_attrs; i++)
				attributes.push_back(mono_method_get_class(attrs->attrs[i].ctor));
			
			return attributes;
		}

		bool monoMethod::hasAttribute(const std::string& name) const
		{
			std::vector<monoClass> attributes = getAttributes();
			for (const monoClass& monoClass : attributes)
				if (monoClass.getName() == name) return true;

			return false;
		}
		monoClass monoMethod::getAttribute(const std::string& name) const
		{
			std::vector<monoClass> attributes = getAttributes();
			for (const monoClass& monoClass : attributes)
				if (monoClass.getName() == name) return monoClass;
			return monoClass();
		}
	}
}

