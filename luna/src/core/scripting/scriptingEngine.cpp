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
	}
}
