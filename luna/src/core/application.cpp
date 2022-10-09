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
			renderer::renderer::createFrame();
			
		}
		application::~application()
		{
			
		}

		void application::run()
		{
			
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
