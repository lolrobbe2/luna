#include "scriptingEngine.h"
#include <core/platform/platformUtils.h>
namespace luna
{
	namespace scripting
	{
		void scriptingEngine::init()
		{
			mono_set_assemblies_path("mono/lib");

			MonoDomain* rootDomain = mono_jit_init("lunaRt");
			if (rootDomain == nullptr)
			{
				LN_CORE_ERROR("could not init jit");
				return;
			}
			s_RootDomain = rootDomain;

			s_AppDomain = mono_domain_create_appdomain("MyAppDomain", nullptr);
			mono_domain_set(s_AppDomain, true);
		}
		void scriptingEngine::shutdown()
		{
		}
		MonoAssembly* scriptingEngine::loadCSharpAssembly(const std::string& assemblyPath)
		{
			std::vector<unsigned char> fileData = platform::os::openFile(assemblyPath);

			if (!fileData.size()) return nullptr;
			// NOTE: We can't use this image for anything other than loading the assembly because this image doesn't have a reference to the assembly
			MonoImageOpenStatus status;
			MonoImage* image = mono_image_open_from_data_full((char*)fileData.data(), fileData.size(), 1, &status, 0);

			if (status != MONO_IMAGE_OK)
			{
				const char* errorMessage = mono_image_strerror(status);
				LN_CORE_ERROR("could not load mono image, error: {0}", errorMessage);
				return nullptr;
			}

			MonoAssembly* assembly = mono_assembly_load_from_full(image, assemblyPath.c_str(), &status, 0);
			mono_image_close(image);

			// Don't forget to free the file data. i wont !
			return assembly;
		}
		uint8_t scriptingEngine::getFieldAccessibility(MonoClassField* field)
		{
			
			//uint32_t accessFlag = mono_field_get_flags(field) & MONO_FIELD_ATTR_FIELD_ACCESS_MASK;
			switch (mono_field_get_flags(field) & MONO_FIELD_ATTR_FIELD_ACCESS_MASK)
			{
			case MONO_FIELD_ATTR_PRIVATE:
				return (uint8_t)accessibility::Private;
			
			case MONO_FIELD_ATTR_FAM_AND_ASSEM:
				return (uint8_t)accessibility::Protected | (uint8_t)accessibility::Internal;
			
			case MONO_FIELD_ATTR_ASSEMBLY:
				return (uint8_t)accessibility::Internal;
			
			case MONO_FIELD_ATTR_FAMILY:
				return (uint8_t)accessibility::Protected;
			
			case MONO_FIELD_ATTR_FAM_OR_ASSEM:
				return (uint8_t)accessibility::Private | (uint8_t)accessibility::Protected;

			case MONO_FIELD_ATTR_PUBLIC:
				return (uint8_t)accessibility::Public;
			}
			return (uint8_t)accessibility::None;
		}
		void scriptingEngine::printAssamblyTypes(MonoAssembly* assembly)
		{
		}
	}
}
