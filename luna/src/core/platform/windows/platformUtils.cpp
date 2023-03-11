#include <core/platform/platformUtils.h>
#ifdef LN_PLATFORM_WINDOWS
#include <commdlg.h>
#include <GLFW/glfw3native.h>
#include <core/application.h>
#include <core/input.h>
namespace luna 
{
	namespace platform
	{
		std::string os::openFilaDialog(const char* filter)
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
			auto* window = static_cast<GLFWwindow*>(application::application::get().getWindow().getWindow());		double xpos, ypos;
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

