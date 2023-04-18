#include <core/platform/platformUtils.h>
#ifdef LN_PLATFORM_WINDOWS
#include <commdlg.h>
#include <GLFW/glfw3native.h>
#include <core/application.h>
#include <core/input.h>
#include <filesystem>

#include <core/object/methodDB.h>
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
			return buffer;
		}

		std::string os::getName()
		{
			return LN_PLATFORM_NAME;
		}

		std::string os::getLocale()
		{
			return std::locale().name();
		}
		
	}
	void Os::RegisterMethods()
	{
		LN_ADD_INTERNAL_CALL(Os, OpenFileDialog);
		LN_ADD_INTERNAL_CALL(Os, SaveFileDialog);
		LN_ADD_INTERNAL_CALL(Os, GetCurrentWorkingDirectory);
		LN_ADD_INTERNAL_CALL(Os, GetName);
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

	MonoString* Os::GetLocale()
	{
		return mono_string_new_wrapper(platform::os::getLocale().c_str());
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

