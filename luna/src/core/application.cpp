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
			
		}
		application::~application()
		{
			
		}

		void application::run()
		{
			while (true)
			{
				mWindow->onUpdate();
			}
		}

		void application::onEvent(Event& e)
		{
			LN_TRACE("{0}", e);
		}
	}
}
