#include <lnpch.h>
#include <core/platform/windows/windowsWindow.h>
#include <core/events/applicationEvent.h>
#include <core/events/keyEvent.h>
#include <core/events/mouseEvent.h>
namespace luna
{
	namespace vulkan
	{
		window* window::windowCreate(const vulkan::windowSpec& windowSpec)
		{
			return new windowsWindow(windowSpec);
		}
		windowsWindow::windowsWindow(const vulkan::windowSpec& windowData)
		{
			init(windowData);
		}
		windowsWindow::~windowsWindow()
		{
			shutDown();
		}
		void windowsWindow::onUpdate()
		{
			glfwPollEvents();
		}
		void windowsWindow::init(const vulkan::windowSpec& windowInfo)
		{
			mData.width = windowInfo.width;
			mData.height = windowInfo.height;
			mData.title = windowInfo.title;

			if (!isGlfwInit)
			{
				int succes = glfwInit();
				window = glfwCreateWindow(mData.width, mData.height, mData.title.c_str(), NULL, NULL);
				isGlfwInit = true;
			}
			glfwSetWindowUserPointer(window, &mData);
			/* setting GLFW callbacks*/
			glfwSetWindowSizeCallback(window, [](GLFWwindow* window, int width, int height)
			{
				windowData& winData = *(windowData*)glfwGetWindowUserPointer(window);
				winData.height = height;
				winData.width = width;
				windowResizeEvent event(width,height);
				winData.eventCallbackFn(event);
			});

			glfwSetWindowCloseCallback(window, [](GLFWwindow* window)
			{
				windowData& winData = *(windowData*)glfwGetWindowUserPointer(window);
				windowCloseEvent event;
				winData.eventCallbackFn(event);
			});

			glfwSetKeyCallback(window, [](GLFWwindow* window, int key, int scancode, int action, int mods)
			{
				windowData& winData = *(windowData*)glfwGetWindowUserPointer(window);
				switch (action)
				{
					case GLFW_RELEASE:
					{
						keyReleasedEvent releasedEvent(key);
						winData.eventCallbackFn(releasedEvent);
					}
					break;
					case GLFW_PRESS:
					{
						keyPressedEvent pressedEvent(key, false);
						winData.eventCallbackFn(pressedEvent);
					}
					break;
					case GLFW_REPEAT:
					{
						keyPressedEvent pressedEvent(key, true);
						winData.eventCallbackFn(pressedEvent);
					}
				}
			});

			glfwSetMouseButtonCallback(window, [](GLFWwindow* window, int button, int action, int mods)
			{
				windowData& winData = *(windowData*)glfwGetWindowUserPointer(window);
				switch (action)
				{
					case GLFW_RELEASE:
					{
						mouseButtonReleasedEvent releasedEvent(button);
						winData.eventCallbackFn(releasedEvent);
					}
					break;
					case GLFW_PRESS:
					{
						mouseButtonPressedEvent pressedEvent(button);
						winData.eventCallbackFn(pressedEvent);
					}
					break;
				}
			});

			glfwSetScrollCallback(window, [](GLFWwindow* window, double xoffset, double yoffset) 
			{
				windowData& winData = *(windowData*)glfwGetWindowUserPointer(window);
				mouseScrolledEvent scrollEvent((float)xoffset, (float)yoffset);
				winData.eventCallbackFn(scrollEvent);
			});

			glfwSetCursorPosCallback(window, [](GLFWwindow* window, double xpos, double ypos)
			{
				windowData& winData = *(windowData*)glfwGetWindowUserPointer(window);
				mouseMovedEvent moveEvent((float)xpos, (float)ypos);
				winData.eventCallbackFn(moveEvent);
			});
		}
		void windowsWindow::shutDown()
		{
			glfwDestroyWindow(window);
			glfwTerminate();
		}
	}
}
