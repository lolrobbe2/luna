#include "CoreCLR.h"
#include <core/debug/debugMacros.h>
#include <core/platform/platformUtils.h>
#include <regex>
namespace luna
{
	namespace scripting
	{
		struct coreCLRFunctionPtrs
		{
			//hostfxr
			hostfxr_main_fn hostfxr_main_ptr = nullptr;
			hostfxr_initialize_for_runtime_config_fn hostfxr_initialize_for_runtime_config_ptr = nullptr;
			hostfxr_get_runtime_delegate_fn hostfxr_get_runtime_delegate_ptr = nullptr;
			hostfxr_main_startupinfo_fn hostfxr_main_startupinfo_ptr = nullptr;
			hostfxr_main_bundle_startupinfo_fn hostfxr_main_bundle_startupinfo_ptr = nullptr;
			hostfxr_run_app_fn hostfxr_run_app_ptr = nullptr;
			hostfxr_close_fn hostfxr_close_ptr = nullptr;
			//coreCLR delegates
			load_assembly_and_get_function_pointer_fn load_assembly_and_get_function_pointer_ptr = nullptr;
			component_entry_point_fn component_entry_point_ptr = nullptr;
			get_function_pointer_fn get_function_pointer_ptr = nullptr;
		};
		static coreCLRFunctionPtrs functionPtrs;

		std::tuple<int, int, int> parseVersion(const std::string& versionStr)
		{
			std::regex versionRegex(R"((\d+)\.(\d+)\.(\d+))");
			std::smatch match;

			if (std::regex_match(versionStr, match, versionRegex))
			{
				int major = std::stoi(match[1]);
				int minor = std::stoi(match[2]);
				int patch = std::stoi(match[3]);
				return { major, minor, patch };
			}
			return { 0, 0, 0 };
		}

		void CoreCLR::init()
		{
			std::filesystem::path coreCLRPath = getCoreCLRPath();
			LN_ERR_FAIL_COND_MSG(std::filesystem::exists(coreCLRPath), "[CoreCLR] CoreCLR is not located ad env path");
			coreCLRLib = platform::os::loadDynamicLib(coreCLRPath);
		}
		void CoreCLR::loadFunctions()
		{
			functionPtrs.hostfxr_main_ptr = (hostfxr_main_fn)coreCLRLib->getProcAddress("hostfxr_main");
			functionPtrs.hostfxr_initialize_for_runtime_config_ptr = (hostfxr_initialize_for_runtime_config_fn)coreCLRLib->getProcAddress("hostfxr_initialize_for_runtime_config");
			functionPtrs.hostfxr_get_runtime_delegate_ptr = (hostfxr_get_runtime_delegate_fn)coreCLRLib->getProcAddress("hostfxr_get_runtime_delegate");
			functionPtrs.hostfxr_main_startupinfo_ptr = (hostfxr_main_startupinfo_fn)coreCLRLib->getProcAddress("hostfxr_main_startupinfo");
			functionPtrs.hostfxr_main_bundle_startupinfo_ptr = (hostfxr_main_bundle_startupinfo_fn)coreCLRLib->getProcAddress("hostfxr_main_bundle_startupinfo");
			functionPtrs.hostfxr_run_app_ptr = (hostfxr_run_app_fn)coreCLRLib->getProcAddress("hostfxr_run_app");
			functionPtrs.hostfxr_close_ptr = (hostfxr_close_fn)coreCLRLib->getProcAddress("hostfxr_close");
		}

		void CoreCLR::hostfxr_main(const int argc, const char_t** argv)
		{
			functionPtrs.hostfxr_main_ptr(argc, argv);
		}
		void CoreCLR::hostfxr_main(const int argc, const char_t** argv)
		{
			functionPtrs.hostfxr_main_ptr(argc, argv);
		}

		void CoreCLR::hostfxr_initialize_for_runtime_config(const char_t* config_path, hostfxr_handle* handle)
		{
			functionPtrs.hostfxr_initialize_for_runtime_config_ptr(config_path, nullptr, handle);
		}

		void CoreCLR::hostfxr_get_runtime_delegate(hostfxr_handle handle, hostfxr_delegate_type type, void** delegate)
		{
			functionPtrs.hostfxr_get_runtime_delegate_ptr(handle, type, delegate);
		}

		void CoreCLR::hostfxr_main_startupinfo(const int argc, const char_t** argv, const char_t* host_path, const char_t* dotnet_root, const char_t* app_path)
		{
			functionPtrs.hostfxr_main_startupinfo_ptr(argc, argv, host_path, dotnet_root, app_path);
		}

		void CoreCLR::hostfxr_main_bundle_startupinfo(const int argc, const char_t** argv, const char_t* host_path, const char_t* dotnet_root, const char_t* app_path, int64_t bundle_header_offset)
		{
			functionPtrs.hostfxr_main_bundle_startupinfo_ptr(argc, argv, host_path, dotnet_root, app_path, bundle_header_offset);
		}

		int CoreCLR::hostfxr_run_app(hostfxr_handle handle)
		{
			return functionPtrs.hostfxr_run_app_ptr(handle);
		}

		void CoreCLR::hostfxr_close(hostfxr_handle handle)
		{
			functionPtrs.hostfxr_close_ptr(handle);
		}

		int CoreCLR::load_assembly_and_get_function_pointer(const char_t* assembly_path, const char_t* type_name, const char_t* method_name, const char_t* delegate_type_name, void* reserved, void** delegate)
		{
			return functionPtrs.load_assembly_and_get_function_pointer_ptr(assembly_path,type_name,method_name,delegate_type_name,reserved,delegate);
		}

		int CoreCLR::component_entry_point(void* arg, int32_t arg_size_in_bytes)
		{
			return functionPtrs.component_entry_point_ptr(arg, arg_size_in_bytes);
		}

		std::filesystem::path CoreCLR::getCoreCLRPath()
		{
			const char* envPath = getenv("CORECLR_PATH");
			//LN_ERR_FAIL_NULL_V_MSG(envPath,"", "[CoreCLR] could not resolve CoreCLR lib Path!");
			if(!envPath) 
			{
				std::filesystem::path path = std::filesystem::path(std::getenv("ProgramFiles"))
				std::filesystem::path hostFxrFolder = path.string() + "/dotnet/hostfxr/";
				return getLatestHostfxrVersionDir(hostFxrFolder);
			}
			return envPath;
		}
		std::filesystem::path CoreCLR::getLatestHostfxrVersionDir(std::filesystem::path& hostfxrPath)
		{
			std::vector<std::filesystem::path> versionDirs;
			for (const auto& entry : std::filesystem::directory_iterator(hostfxrPath))
			{
				if (entry.is_directory()) {
					std::string dirName = entry.path().filename().string();
					// Ensure the directory name is a valid version string before adding it
					if (std::regex_match(dirName, std::regex(R"(\d+\.\d+\.\d+)"))) 
						versionDirs.push_back(entry.path());
					
				}
			}

			LN_ERR_FAIL_NULL_V_MSG(!versionDirs.empty(),"","[CoreCLR] No version directories found in {0}", hostfxrPath);

			// Sort directories by version number
			std::sort(versionDirs.begin(), versionDirs.end(), [](const std::filesystem::path& a, const std::filesystem::path& b) {
				return parseVersion(a.filename().string()) < parseVersion(b.filename().string());
				});
		}
	}
}