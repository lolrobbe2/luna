#include <core/platform/platformUtils.h>
#ifdef LN_PLATFORM_WINDOWS
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
		std::string os::openFileDialog(const char* filter)
		{
			OPENFILENAMEA ofn;
			CHAR szFile[260] = { 0 };
			CHAR currentDir[256] = { 0 };
			ZeroMemory(&ofn, sizeof(OPENFILENAME));
			ofn.lStructSize = sizeof(OPENFILENAME);
			ofn.hwndOwner = glfwGetWin32Window((GLFWwindow*) application::application::get().getWindow().getWindow());
			ofn.lpstrFile = szFile;
			ofn.nMaxFile = sizeof(szFile);
			if (GetCurrentDirectoryA(256, currentDir))
				ofn.lpstrInitialDir = currentDir;
			ofn.lpstrFilter = filter;
			ofn.nFilterIndex = 1;
			ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST | OFN_NOCHANGEDIR;

			if (GetOpenFileNameA(&ofn) == TRUE)
				return ofn.lpstrFile;

			return std::string();
		}
		std::string os::saveFileDialog(const char* filter)
		{
			OPENFILENAMEA ofn;
			CHAR szFile[260] = { 0 };
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

			if (!stream)
			{
				LN_CORE_ERROR("could not open file at {0}", filePath);
				return std::vector<unsigned char>();
			}

			std::streampos end = stream.tellg();
			stream.seekg(0, std::ios::beg);
			uint32_t size = end - stream.tellg();

			if (size == 0)
			{
				LN_CORE_ERROR("file is empty at {0}", filePath);
				return std::vector<unsigned char>();
			}

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
			char buffer[FILENAME_MAX];
			_getcwd(buffer, FILENAME_MAX);
			std::string workingDir = std::string(buffer, buffer+FILENAME_MAX);
			while (workingDir.find("\\") != std::string::npos) workingDir.replace(workingDir.find("\\"), sizeof("\\") - 1, "/");

			return workingDir;
		}

		std::string os::getVersion() {
			typedef LONG NTSTATUS;
			typedef NTSTATUS(WINAPI* RtlGetVersionPtr)(PRTL_OSVERSIONINFOW);
			RtlGetVersionPtr version_ptr = (RtlGetVersionPtr)GetProcAddress(GetModuleHandleA("ntdll.dll"), "RtlGetVersion");
			if (version_ptr != nullptr) {
				RTL_OSVERSIONINFOW fow;
				ZeroMemory(&fow, sizeof(fow));
				fow.dwOSVersionInfoSize = sizeof(fow);
				if (version_ptr(&fow) == 0x00000000) {
					return fmt::format("{0}.{1}.{2}", (int64_t)fow.dwMajorVersion, (int64_t)fow.dwMinorVersion, (int64_t)fow.dwBuildNumber);
				}
			}
			return "";
		}

		std::string os::getName()
		{
			return LN_PLATFORM_NAME;
		}

		std::string os::getLocale()
		{
			LCID lcid = GetThreadLocale();
			wchar_t name[LOCALE_NAME_MAX_LENGTH];
			if (LCIDToLocaleName(lcid, name, LOCALE_NAME_MAX_LENGTH, 0) == 0)
				LN_CORE_ERROR("an error occured",GetLastError());
			else {
				std::wstring ws(name);
				return std::string(ws.begin(), ws.end());
			}
			return "en";
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
			return _getpid();;
		}

		std::string os::getExecutablePath() {
			WCHAR bufname[4096];
			GetModuleFileNameW(nullptr, bufname, 4096);
			std::wstring ws(bufname);
			std::string executablePath = std::string(ws.begin(), ws.end());
			while(executablePath.find("\\") != std::string::npos) executablePath.replace(executablePath.find("\\"), sizeof("\\") - 1, "/");
			return executablePath;
		}
		void os::setConsoleVisibility(bool visible)
		{
			::ShowWindow(::GetConsoleWindow(), visible ? SW_HIDE : SW_SHOW);
		}
		void os::createConsole()
		{
			AllocConsole();
			AttachConsole(GetCurrentProcessId());
			freopen("CON", "w", stdout);
			freopen("CON", "w", stderr);
			freopen("CON", "r", stdin);
			Log::Reinit();
			LN_CORE_INFO("attached console window");
		}
	}
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

