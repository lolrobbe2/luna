#include <core/platform/platformUtils.h>
#ifdef LN_PLATFORM_WINDOWS

#include <shlobj_core.h>


#include <commdlg.h>
#include <GLFW/glfw3native.h>
#include <core/application.h>
#include <core/input.h>
#include <filesystem>
#include <locale.h>
#include <core/object/methodDB.h>
#include <string_view>

namespace luna 
{
	namespace platform
	{
		const static std::filesystem::path engineRootPath = std::filesystem::current_path();

		std::string os::openFileDialog(const char* filter)
		{
			OPENFILENAMEA ofn;
			CHAR szFile[MAX_PATH] = { 0 };
			CHAR currentDir[256] = { 0 };
			ZeroMemory(&ofn, sizeof(OPENFILENAME));
			ofn.lStructSize = sizeof(OPENFILENAME);
			ofn.hwndOwner = glfwGetWin32Window((GLFWwindow*) application::application::get().getWindow().getWindow());
			ofn.lpstrFile = szFile;
			ofn.nMaxFile = MAX_PATH;
			if (GetCurrentDirectoryA(256, currentDir))
				ofn.lpstrInitialDir = currentDir;
			ofn.lpstrFilter = filter;
			ofn.nFilterIndex = 1;
			ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST | OFN_NOCHANGEDIR;

			if (GetOpenFileNameA(&ofn) == TRUE)
				return ofn.lpstrFile;

			return std::string();
		}
		std::string os::openFolderDialog()
		{
			std::string selectedFolder = "";

			// Create an instance of the File Open Dialog
			IFileDialog* pFileDialog = nullptr;
			HRESULT hr = CoCreateInstance(CLSID_FileOpenDialog, nullptr, CLSCTX_INPROC_SERVER, IID_PPV_ARGS(&pFileDialog));
			if (SUCCEEDED(hr))
			{
				// Set options to select folders only
				DWORD options;
				pFileDialog->GetOptions(&options);
				pFileDialog->SetOptions(options | FOS_PICKFOLDERS);

				// Show the dialog
				if (SUCCEEDED(pFileDialog->Show(nullptr)))
				{
					// Get the selected folder path
					IShellItem* pResult = nullptr;
					if (SUCCEEDED(pFileDialog->GetResult(&pResult)))
					{
						PWSTR folderPath;
						if (SUCCEEDED(pResult->GetDisplayName(SIGDN_FILESYSPATH, &folderPath)))
						{
							// Convert the wide string to narrow string
							int bufferSize = WideCharToMultiByte(CP_UTF8, 0, folderPath, -1, nullptr, 0, nullptr, nullptr);
							if (bufferSize > 0)
							{
								std::string narrowPath(bufferSize, '\0');
								WideCharToMultiByte(CP_UTF8, 0, folderPath, -1, narrowPath.data(), bufferSize, nullptr, nullptr);
								selectedFolder = narrowPath;
							}
							CoTaskMemFree(folderPath);
						}
						pResult->Release();
					}
				}

				pFileDialog->Release();
			}

			return selectedFolder;
		}
		std::string os::saveFileDialog(const char* filter)
		{
			OPENFILENAMEA ofn;
			CHAR szFile[MAX_PATH] = { 0 };
			CHAR currentDir[256] = { 0 };
			ZeroMemory(&ofn, sizeof(OPENFILENAME));
			ofn.lStructSize = sizeof(OPENFILENAME);
			ofn.hwndOwner = glfwGetWin32Window((GLFWwindow*)application::application::get().getWindow().getWindow());
			ofn.lpstrFile = szFile;
			ofn.nMaxFile = sizeof(szFile);
			if (GetCurrentDirectoryA(256, currentDir))
				ofn.lpstrInitialDir = currentDir;
			ofn.lpstrFilter = filter;
			ofn.nFilterIndex = 1;
			ofn.Flags = OFN_PATHMUSTEXIST | OFN_OVERWRITEPROMPT | OFN_NOCHANGEDIR;

			// Sets the default extension by extracting it from the filter
			ofn.lpstrDefExt = strchr(filter, '\0') + 1;

			if (GetSaveFileNameA(&ofn) == TRUE)
				return std::string(ofn.lpstrFile);

			return std::string();
		}
		std::vector<unsigned char> os::openFile(const std::string& filePath)
		{
			std::ifstream stream(filePath, std::ios::binary | std::ios::ate);

			LN_ERR_FAIL_NULL_V_MSG(stream, std::vector<unsigned char>(), "could not open file at " + filePath);

			std::streampos end = stream.tellg();
			stream.seekg(0, std::ios::beg);
			uint32_t size = end - stream.tellg();

			LN_ERR_FAIL_NULL_V_MSG(size, std::vector<unsigned char>(), "file is empty at " + filePath);

			unsigned char* buffer = new unsigned char[size];
			stream.read((char*)buffer, size);
			stream.close();
			std::vector<unsigned char> file;
			file.resize(size);
			memcpy_s(file.data(), file.size(), buffer, size);
			return file;
		}
	
		std::string os::getCurrentWorkingDirectory()
		{
			return std::filesystem::current_path().string();
		}

		std::string os::getVersion() {
			typedef LONG NTSTATUS;
			typedef NTSTATUS(WINAPI* RtlGetVersionPtr)(PRTL_OSVERSIONINFOW);
			RtlGetVersionPtr version_ptr = (RtlGetVersionPtr)GetProcAddress(GetModuleHandleA("ntdll.dll"), "RtlGetVersion");
			LN_ERR_FAIL_NULL_V_MSG(version_ptr, std::string(), "version_ptr was null");
			RTL_OSVERSIONINFOW fow;
			ZeroMemory(&fow, sizeof(fow));
			fow.dwOSVersionInfoSize = sizeof(fow);
			if (version_ptr(&fow) == 0x00000000) {
				return fmt::format("{0}.{1}.{2}", (int64_t)fow.dwMajorVersion, (int64_t)fow.dwMinorVersion, (int64_t)fow.dwBuildNumber);
			}
		}

		std::string os::getName()
		{
			return LN_PLATFORM_NAME;
		}

		std::string os::getLocale()
		{
			LCID lcid = GetThreadLocale();
			wchar_t name[LOCALE_NAME_MAX_LENGTH];
			LN_ERR_FAIL_NULL_V_MSG(LCIDToLocaleName(lcid, name, LOCALE_NAME_MAX_LENGTH, 0), "en", "an error occured" + GetLastError());
			std::wstring ws(name);
			return std::string(ws.begin(), ws.end());
		}
		
		std::string os::getLocaleLanguage() {
			
			std::stringstream locale(getLocale());
			std::vector<std::string> languageSeglist;
			std::string segment;
			while (std::getline(locale, segment, '-')) languageSeglist.push_back(segment);
			return languageSeglist.front();
		}

		int os::getProcessId()
		{
			return _getpid();
		}

		std::string os::getExecutablePath() {
			WCHAR bufname[4096];
			GetModuleFileNameW(nullptr, bufname, 4096);
			std::wstring ws(bufname);
			std::string executablePath = std::string(ws.begin(), ws.end());
			while(executablePath.find("\\") != std::string::npos) executablePath.replace(executablePath.find("\\"), sizeof("\\") - 1, "/");
			return executablePath;
		}

		void os::setCursorShape(const cursorShape shape) 
		{
			switch (shape)
			{
			case ARROW:
				SetSystemCursor(LoadCursor(NULL, IDC_ARROW), 32512);
				break;
			case IBEAM:
				SetSystemCursor(LoadCursor(NULL, IDC_IBEAM), 32512);
				break;
			case CROSSHAIR:
				SetSystemCursor(LoadCursor(NULL, IDC_CROSS), 32512);
				break;
			case HAND:
				SetSystemCursor(LoadCursor(NULL, IDC_ARROW), 32512);
				break;
			case HRESIZE:
				SetSystemCursor(LoadCursor(NULL, IDC_SIZEWE), 32512);
				break;
			case VRESIZE:
				SetSystemCursor(LoadCursor(NULL, IDC_SIZENS), 32512);
				break;
			case APP_STARTING:
				SetSystemCursor(LoadCursor(NULL, IDC_APPSTARTING), 32512);
				break;
			case WAITING:
				SetSystemCursor(LoadCursor(NULL, IDC_WAIT), 32512);
				break;
			default:
				SetSystemCursor(LoadCursor(NULL, IDC_ARROW), 32512);
				break;
			}

		}

		std::string filesystem::getSystemFolderPath(const folderTypes folderType)
		{
			//uses shlobj_core.h
			wchar_t Folder[MAX_PATH];
			int CSIDL;
			switch (folderType)
			{
			case luna::platform::desktop:
				CSIDL = CSIDL_DESKTOP;
				break;
			case luna::platform::desktopDir:
				CSIDL = CSIDL_DESKTOPDIRECTORY;
				break;
			case luna::platform::documents:
				CSIDL = CSIDL_MYDOCUMENTS;
				break;
			case luna::platform::music:
				CSIDL = CSIDL_MYMUSIC;
				break;
			case luna::platform::video:
				CSIDL = CSIDL_MYVIDEO;
				break;
			case luna::platform::fonts:
				CSIDL = CSIDL_FONTS;
				break;
			case luna::platform::appData:
				CSIDL = CSIDL_APPDATA;
				break;
			case luna::platform::root:
				return "C:\\";
				break;
			case luna::platform::programFiles:
				CSIDL = CSIDL_PROGRAM_FILES;
				break;
			case luna::platform::programFilesX86:
				CSIDL = CSIDL_PROGRAM_FILESX86;
				break;
			case luna::platform::recycleBin:
				CSIDL = CSIDL_BITBUCKET;
				break;
			default:
				LN_CORE_ERROR("could not find special folder!");
				return "meep meep could not find anything!";
			}
			HRESULT hr = SHGetFolderPathW(0, CSIDL, 0, 0, Folder);
			if (SUCCEEDED(hr))
			{
				char str[MAX_PATH];
				wcstombs(str, Folder, MAX_PATH - 1);
				return str;
			}
			LN_CORE_ERROR("could not find special folder!");
			return "meep meep could not find anything!";
		}
		std::filesystem::path filesystem::getEngineRootPath()
		{
			return engineRootPath;
		}
		
}

#pragma region osGlue
	void Os::RegisterMethods()
	{
		LN_ADD_INTERNAL_CALL(Os, OpenFileDialog);
		LN_ADD_INTERNAL_CALL(Os, SaveFileDialog);
		LN_ADD_INTERNAL_CALL(Os, GetCurrentWorkingDirectory);
		LN_ADD_INTERNAL_CALL(Os, GetName);
		LN_ADD_INTERNAL_CALL(Os, GetVersion);
		LN_ADD_INTERNAL_CALL(Os, GetLocale);
		LN_ADD_INTERNAL_CALL(Os, GetLocaleLanguage);
		LN_ADD_INTERNAL_CALL(Os, GetProcessId);
	}

	MonoString* Os::OpenFileDialog(MonoString* filter)
	{
		std::string filePath = platform::os::openFileDialog(mono_string_to_utf8(filter));
		return mono_string_new_wrapper(filePath.c_str());
	}
	MonoString* Os::SaveFileDialog(MonoString* filter)
	{
		std::string filePath = platform::os::saveFileDialog(mono_string_to_utf8(filter));
		return mono_string_new_wrapper(filePath.c_str());
	}

	MonoString* Os::GetCurrentWorkingDirectory()
	{
		return mono_string_new_wrapper(platform::os::getCurrentWorkingDirectory().c_str());
	}

	MonoString* Os::GetName() 
	{
		return mono_string_new_wrapper(platform::os::getName().c_str());
	}

	MonoString* Os::GetVersion()
	{
		return mono_string_new_wrapper(platform::os::getVersion().c_str());
	}

	MonoString* Os::GetLocale()
	{
		return mono_string_new_wrapper(platform::os::getLocale().c_str());
	}

	MonoString* Os::GetLocaleLanguage()
	{
		return mono_string_new_wrapper(platform::os::getLocaleLanguage().c_str());
	}
	int Os::GetProcessId() {
		return platform::os::getProcessId();
	}
#pragma endregion
}



namespace luna 
{
	namespace input
	{
		bool input::isKeyPressed(const keyCode key)
		{
			auto* window = static_cast<GLFWwindow*>(application::application::get().getWindow().getWindow());
			auto state = glfwGetKey(window, static_cast<int32_t>(key));
			return state == GLFW_PRESS;
		}

		bool input::isMouseButtonPressed(const mouseCode button)
		{
			auto* window = static_cast<GLFWwindow*>(application::application::get().getWindow().getWindow());
			auto state = glfwGetMouseButton(window, static_cast<int32_t>(button));
			return state == GLFW_PRESS;
		}

		glm::vec2 input::getMousePosition()
		{
			auto* window = static_cast<GLFWwindow*>(application::application::get().getWindow().getWindow());		
			double xpos, ypos;
			glfwGetCursorPos(window, &xpos, &ypos);

			return { (float)xpos, (float)ypos };
		}

		float input::getMouseX()
		{
			return getMousePosition().x;
		}

		float input::getMouseY()
		{
			return getMousePosition().y;
		}
	}
}


#endif // LN_PLATFORM_WINDOWS

