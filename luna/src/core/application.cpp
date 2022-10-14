#pragma once
#include <lnpch.h>
#include <core/application.h>
namespace luna
{
	namespace application
	{
		application::application()
		{
			Log::Init();
			mWindow = ref<vulkan::window>(vulkan::window::windowCreate());
			mWindow->setEventCallBack(LN_BIND_EVENT_FN(onEvent));
			renderer::renderer::init(mWindow);

			
		}
		application::~application()
		{
			
		}

		void application::run()
		{
			renderer::renderer::createFrame();
			while (running)
			{
				
				mWindow->onUpdate();
				renderer::renderer::newFrame();
			}
		}

		void application::onEvent(Event& e)
		{
			if (e.getEventType() == luna::eventType::WindowClose) running = false;
		}
	}
}
