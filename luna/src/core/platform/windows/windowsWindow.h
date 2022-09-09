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
			windowsWindow(const vulkan::windowSpec& windowData );
			virtual ~windowsWindow();

			void onUpdate() override;
			inline uint32_t getWidth() const override { return mData.width; };
			inline uint32_t getHeight() const override { return mData.height; };
			inline void setEventCallBack(const eventCallbackFn& callback) override { mData.eventCallbackFn = callback; };
			inline void* getWindow() override;
		private: 
			virtual void init(const vulkan::windowSpec& windowInfo);
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




