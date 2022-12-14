#pragma once
#ifndef WINDOW
#define WINDOW

#include <lnpch.h>
#include <core/core.h>
#include <core/events/event.h>

namespace luna
{
	namespace vulkan
	{
		
		struct windowSpec
		{
			uint32_t width;
			uint32_t height;
			std::string title;
			windowSpec(const std::string& Title = "Luna Engine", uint32_t Width = 1280, uint32_t Height = 720) : title(Title), width(Width), height(Height)
			{
			};
		};

		
		enum Api
		{
			NONE = 0,
			VULKAN = 1,
			OPENGL = 2
		};
		//using renderApi = uint8_t;
		
		/**
		*
		*	@brief this is the window class en creates an os window
		*	@class std::shared_ptr<window>(window::create(windowSpec windowSpec));
		*	@warning use as an std::shared_ptr /ref
		*/
		class window
		{
			
		public:
			
			static Api graphicsApi;

			using eventCallbackFn = std::function<void(Event&)>;

			virtual ~window() {};
			static window* windowCreate(const windowSpec& WindowSpec = windowSpec::windowSpec());
			virtual void onUpdate() = 0;
			virtual uint32_t getWidth() const = 0;
			virtual uint32_t getHeight() const = 0;
			virtual void setEventCallBack(const eventCallbackFn& callback) = 0;
			virtual void* getWindow() = 0;
			windowSpec windowSpec;
		};
	}
}
#endif // !WINDOW

