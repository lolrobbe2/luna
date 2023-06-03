#include <lnpch.h>
#include <core/platform/windows/windowsWindow.h>
#include <core/events/applicationEvent.h>
#include <core/events/keyEvent.h>
#include <core/events/mouseEvent.h>
#ifndef DISABLE_IMGUI
#include <backends/imgui_impl_glfw.h>
#endif //DISABLE_IMGUI
namespace luna
{
	namespace vulkan
	{
		Api window::graphicsApi = Api::VULKAN;
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
			LN_PROFILE_FUNCTION() ;
			glfwPollEvents();
		}
		inline void* windowsWindow::getWindow()
		{
			return _window;
		}
		glm::vec2 windowsWindow::getCursorPos() const
		{
			double xpos, ypos;
			glfwGetCursorPos(_window, &xpos, &ypos);
			return {xpos,ypos};
		}
		void windowsWindow::init(const vulkan::windowSpec& windowInfo)
		{
			LN_PROFILE_FUNCTION();
			mData.width = windowInfo.width;
			mData.height = windowInfo.height;
			mData.title = windowInfo.title;
		
			if (!isGlfwInit)
			{
				int succes = glfwInit();
				glfwSetErrorCallback([](int error_code, const char* description)
				{
						LN_CORE_ERROR("glfw error. error code = {0}, description: {1}", error_code, description);
				});
				isGlfwInit = true;

			}

			//select GLFW_API based on selected API
			switch (graphicsApi)
			{
			case luna::vulkan::NONE:
				break;
			case luna::vulkan::VULKAN:
				glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
				break;
			case luna::vulkan::OPENGL:
				glfwWindowHint(GLFW_CLIENT_API, GLFW_OPENGL_API);
				break;
			default:
				break;
			}
			
			_window = glfwCreateWindow(mData.width, mData.height, mData.title.c_str(), NULL, NULL);

			glfwSetWindowUserPointer(_window, &mData);
			/* setting GLFW callbacks*/
			glfwSetWindowSizeCallback(_window, [](GLFWwindow* window, int width, int height)
			{
				windowData& winData = *(windowData*)glfwGetWindowUserPointer(window);
				winData.height = height;
				winData.width = width;
				windowResizeEvent event(width,height);
				winData.eventCallbackFn(event);
				
			});

			glfwSetWindowFocusCallback(_window, [](GLFWwindow* window, int focused) 
			{
				windowData& winData = *(windowData*)glfwGetWindowUserPointer(window);
				windowFocusEvent focusEvent;
				windowLostFocusEvent lostFocusEvent;
				if (focused) winData.eventCallbackFn(focusEvent);
				else winData.eventCallbackFn(lostFocusEvent);
					
			});

			glfwSetWindowCloseCallback(_window, [](GLFWwindow* window)
			{
				windowData& winData = *(windowData*)glfwGetWindowUserPointer(window);
				windowCloseEvent event;
				winData.eventCallbackFn(event);
			});

			glfwSetKeyCallback(_window, [](GLFWwindow* window, int key, int scancode, int action, int mods)
			{
				windowData& winData = *(windowData*)glfwGetWindowUserPointer(window);
			#ifndef DISABLE_IMGUI
				ImGui_ImplGlfw_KeyCallback(window, key, scancode, action, mods);
			#endif //DISABLE_IMGUI
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

			glfwSetMouseButtonCallback(_window, [](GLFWwindow* window, int button, int action, int mods)
			{
				windowData& winData = *(windowData*)glfwGetWindowUserPointer(window);
			#ifndef DISABLE_IMGUI
				ImGui_ImplGlfw_MouseButtonCallback(window, button, action, mods);
			#endif //DISABLE_IMGUI
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
						static int prevButton;
						static auto prevTime = std::chrono::system_clock::now();
						auto currentTime = std::chrono::system_clock::now();
						double diff_ms = std::chrono::duration <double, std::milli>(currentTime - prevTime).count();
						prevTime = currentTime;
						if (diff_ms > 10 && diff_ms < 200 && prevButton == button) {
							mouseButtonPressedEvent pressedEvent(button,true);
						}
						else
						{
							mouseButtonPressedEvent pressedEvent(button, false);
							winData.eventCallbackFn(pressedEvent);
						}
						prevButton = button;
						
			
					}
					break;
				}
			});

			glfwSetScrollCallback(_window, [](GLFWwindow* window, double xoffset, double yoffset) 
			{
				windowData& winData = *(windowData*)glfwGetWindowUserPointer(window);
			#ifndef DISABLE_IMGUI
				ImGui_ImplGlfw_ScrollCallback(window, xoffset, yoffset);
			#endif //DISABLE_IMGUI
				mouseScrolledEvent scrollEvent((float)xoffset, (float)yoffset);
				winData.eventCallbackFn(scrollEvent);
			});

			glfwSetCursorPosCallback(_window, [](GLFWwindow* window, double xpos, double ypos)
			{
				windowData& winData = *(windowData*)glfwGetWindowUserPointer(window);
			#ifndef DISABLE_IMGUI
				ImGui_ImplGlfw_CursorPosCallback(window, xpos, ypos);
			#endif //DISABLE_IMGUI
				mouseMovedEvent moveEvent((float)xpos, (float)ypos);
				winData.eventCallbackFn(moveEvent);
			});
			//imgui specific callbacks
			#ifndef DISABLE_IMGUI
			glfwSetCharCallback(_window, [](GLFWwindow* window, unsigned int codepoint)
			{
				ImGui_ImplGlfw_CharCallback(window, codepoint);
			});
			#endif //DISABLE_IMGUI
		}
		void windowsWindow::shutDown()
		{
			glfwDestroyWindow(_window);
			glfwTerminate();
		}
	}
}
