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
			char buff[FILENAME_MAX];
			return _getcwd(buff, FILENAME_MAX);
		}
		void os::bindStaticFunctions()
		{
			LN_ADD_INTERNAL_CALL(os, luna::os::OpenFileDialogInternal);
			LN_ADD_INTERNAL_CALL(os, luna::os::SaveFileDialogInternal);
		}
	}
	MonoString* os::OpenFileDialogInternal(MonoString* filter)
	{
		std::string filePath = platform::os::openFileDialog(mono_string_to_utf8(filter));
		return mono_string_new_wrapper(filePath.c_str());
	}
	MonoString* os::SaveFileDialogInternal(MonoString* filter)
	{
		std::string filePath = platform::os::saveFileDialog(mono_string_to_utf8(filter));
		return mono_string_new_wrapper(filePath.c_str());
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

