#pragma once
#ifndef WINDOWS_WINDOW
#define WINDOWS_WINDOW
#include <core/vulkan/window/window.h>
namespace luna
{
	namespace vulkan
	{
		static bool isGlfwInit;
		class windowsWindow : public vulkan::window
		{
			
		public:
			/**
			 * @brief creates a windows window.
			 * 
			 * \param windowData windowSpec struct with the window data
			 */
			windowsWindow(const vulkan::windowSpec& windowData );
			virtual ~windowsWindow();

			/**
			 * @brief polls all the window and input events.
			 * 
			 */
			void onUpdate() override;
			/**
			 * @brief returs the current width of the window.
			 */
			inline uint32_t getWidth() const override { return mData.width; };
			/**
			 * @brief return the current height of the window.
			 */
			inline uint32_t getHeight() const override { return mData.height; };
			/**
			 * @brief set the window event callback function.
			 */
			inline void setEventCallBack(const eventCallbackFn& callback) override { mData.eventCallbackFn = callback; };
			/**
			 * @brief returns the GLFW window pointer.
			 * 
			 * \return 
			 */
			inline void* getWindow() override;
			/**
			 * @brief returns the current cursor pos.
			 * 
			 * \return glm::vec2
			 */
			virtual glm::vec2 getCursorPos() const override;
			virtual void setCursorShape(const cursorShape shape) const override;
		private: 
			/**
			 * initializes the window and also GLFW if need be.
			 * 
			 * \param windowInfo
			 */
			virtual void init(const vulkan::windowSpec& windowInfo);
			/**
			 * @brief shutsdown the window and GLFW.
			 */
			virtual void shutDown();

		private:
			
			GLFWwindow* _window;
			struct windowData
			{
				uint32_t width = 0;
				uint32_t height = 0;
				std::string title{};
				eventCallbackFn eventCallbackFn;
			};
			windowData mData;
		};
	}
}
#endif // !WINDOWS_WINDOW




