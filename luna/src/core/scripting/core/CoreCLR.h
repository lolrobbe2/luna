#pragma once
#ifndef _CORE_CLR_
#define _CORE_CLR_
#include <core/core.h>
#include <core/thirdParty/coreclr_delegates.h>
#include <core/thirdParty/hostfxr.h>
#include <core/platform/dynamicLibrary.h>
namespace luna
{
	
	namespace scripting
	{
		class CoreCLR
		{
		public:
			void init();
		private:
			void loadFunctions();

			void hostfxr_main(const int argc, const char_t** argv);
			void hostfxr_initialize_for_runtime_config(const char_t* config_path, hostfxr_handle* handle);
			void hostfxr_get_runtime_delegate(hostfxr_handle handle, hostfxr_delegate_type type, void** delegate);
			void hostfxr_main_startupinfo(const int argc,
				const char_t** argv,
				const char_t* host_path,
				const char_t* dotnet_root,
				const char_t* app_path);
			void hostfxr_main_bundle_startupinfo(const int argc,
				const char_t** argv,
				const char_t* host_path,
				const char_t* dotnet_root,
				const char_t* app_path,
				int64_t bundle_header_offset);
			int hostfxr_run_app(hostfxr_handle handle);
			void hostfxr_close(hostfxr_handle handle);

			int load_assembly_and_get_function_pointer(
				const char_t* assembly_path      /* Fully qualified path to assembly */,
				const char_t* type_name          /* Assembly qualified type name */,
				const char_t* method_name        /* Public static method name compatible with delegateType */,
				const char_t* delegate_type_name /* Assembly qualified delegate type name or null
													or UNMANAGEDCALLERSONLY_METHOD if the method is marked with
													the UnmanagedCallersOnlyAttribute. */,
				void* reserved           /* Extensibility parameter (currently unused and must be 0) */,
				/*out*/ void** delegate          /* Pointer where to store the function pointer result */);
			int component_entry_point(void* arg, int32_t arg_size_in_bytes);
			ref<platform::dynamicLibrary> coreCLRLib = nullptr;
			std::filesystem::path getCoreCLRPath();
			std::filesystem::path getLatestHostfxrVersionDir(std::filesystem::path& hostfxrPath);
		};
	}
}
#endif // !_CORE_CLR_
