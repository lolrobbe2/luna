#include "scriptingEngine.h"
#include <core/platform/platformUtils.h>
#include <mono/metadata/mono-debug.h>
#include <mono/metadata/threads.h>
namespace luna
{
	namespace scripting
	{
		
		struct scriptEngineData 
		{
			MonoDomain* rootDomain;
			MonoDomain* appDomain;
			MonoAssembly* coreAssembly;
			MonoImage* coreImage;

			MonoAssembly* appAssembly;
			MonoImage* appImage;
			scene* m_Context;

			std::filesystem::path coreAssemblyFilepath;
			std::filesystem::path appAssemblyFilepath;

			bool enableDebugging = false;

			scene* sceneContext = nullptr;
		};

		static scriptEngineData* s_Data = nullptr;

		void scriptingEngine::init()
		{
			initMono();

			if (!loadAssembly("mono/lib/scriptCore.dll")) return LN_CORE_ERROR("[scriptingEngine] could not load scriptCore.dll");
			if (!loadAppAssembly("mono/lib/sharpSandbox.dll")) return LN_CORE_ERROR("[scriptingEngine] could not load app assembly dll");
			
			LN_CORE_INFO("started scriptingEngine");
		}

		void scriptingEngine::shutdown()
		{
			mono_domain_set(mono_get_root_domain(), false);

			mono_domain_unload(s_Data->appDomain);
			s_Data->appDomain = nullptr;

			mono_jit_cleanup(s_Data->rootDomain);
			s_Data->rootDomain = nullptr;

			delete s_Data;
		}

		void scriptingEngine::initMono() 
		{
			s_Data = new scriptEngineData();
			mono_set_dirs(platform::os::getCurrentWorkingDirectory().c_str(), "mono/etc");

			if (s_Data->enableDebugging)
			{
				const char* argv[2] = {
					"--debugger-agent=transport=dt_socket,address=127.0.0.1:2550,server=y,suspend=n,loglevel=3,logfile=MonoDebugger.log",
					"--soft-breakpoints"
				};

				mono_jit_parse_options(2, (char**)argv);
				mono_debug_init(MONO_DEBUG_FORMAT_MONO);
			}

			MonoDomain* rootDomain = mono_jit_init("LunaJitRt");
			if (!rootDomain) LN_CORE_INFO("could not init LunaJitRt");

			// Store the root domain pointer
			s_Data->rootDomain = rootDomain;

			if (s_Data->enableDebugging)
				mono_debug_domain_create(s_Data->rootDomain);

			mono_thread_set_main(mono_thread_current());
		}

		bool scriptingEngine::loadAssembly(const std::filesystem::path& filepath)
		{
			// Create an App Domain
			s_Data->appDomain = mono_domain_create_appdomain("lunaScriptRt", nullptr);
			mono_domain_set(s_Data->appDomain, true);

			s_Data->coreAssemblyFilepath = filepath;//todo pdb
			s_Data->coreAssembly = loadCSharpAssembly(filepath.generic_string());
			if (!s_Data->coreAssembly)
				return false;

			s_Data->coreImage = mono_assembly_get_image(s_Data->coreAssembly);
			return true;
		}

		bool scriptingEngine::loadAppAssembly(const std::filesystem::path& filepath)
		{
			s_Data->appAssemblyFilepath = filepath;
			s_Data->appAssembly = loadCSharpAssembly(filepath.generic_string());
			if (!s_Data->appAssembly)
				return false;

			s_Data->appImage = mono_assembly_get_image(s_Data->appAssembly);
			/* TODO hot reloading code
			s_Data->appAssemblyFileWatcher = createScope<filewatch::FileWatch<std::string>>(filepath.string(), OnAppAssemblyFileSystemEvent);
			s_Data->assemblyReloadPending = false;
			*/
			return true;
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
			MonoImage* image = mono_assembly_get_image(assembly);
			const MonoTableInfo* typeDefinitionsTable = mono_image_get_table_info(image, MONO_TABLE_TYPEDEF);
			int32_t numTypes = mono_table_info_get_rows(typeDefinitionsTable);

			for (int32_t i = 0; i < numTypes; i++)
			{
				uint32_t cols[MONO_TYPEDEF_SIZE];
				mono_metadata_decode_row(typeDefinitionsTable, i, cols, MONO_TYPEDEF_SIZE);

				const char* nameSpace = mono_metadata_string_heap(image, cols[MONO_TYPEDEF_NAMESPACE]);
				const char* name = mono_metadata_string_heap(image, cols[MONO_TYPEDEF_NAME]);
				LN_CORE_INFO("type def: {0}.{1}", nameSpace, name);
			}
		}

		scene* scriptingEngine::getContext()
		{
			return s_Data->m_Context;
		}


		rootClass::rootClass(MonoClass* childClass)
		{
			
		}

		scriptClass::scriptClass(std::string nodeName)
		{
			mono_class_from_name(s_Data->appImage, "luna", nodeName.c_str());
			readyMethod = mono_class_get_method_from_name(baseClass, "ready", 0);
			processMethod = mono_class_get_method_from_name(baseClass, "process", 1);
			physicsProcessMethod = mono_class_get_method_from_name(baseClass, "physicsProcess", 1);

		}

}
}
