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
			LN_CORE_INFO("size of buffer handle = {0}",sizeof(VkBuffer));
			LN_CORE_INFO("size of commandBuffer handle = {0}", sizeof(VkCommandBuffer));
			while (running)
			{
				renderer::renderer::newFrame();
				mWindow->onUpdate();
			}
		}

		void application::onEvent(Event& e)
		{
			if (e.getEventType() == luna::eventType::WindowClose) running = false;
		}
	}
}
