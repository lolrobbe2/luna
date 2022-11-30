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
			renderer::renderer2D::init();

			
		}
		application::~application()
		{
			renderer::renderer2D::shutdown();
		}

		void application::run()
		{
			renderer::renderer::createFrame();
			
			while (running)
			{
				
				mWindow->onUpdate();
				renderer::renderer2D::BeginScene();
				renderer::renderer2D::drawQuad({ 0.0f,0.0f ,0.0f }, { 1.0f ,1.0f });
				renderer::renderer2D::endScene();
				renderer::renderer::newFrame();
			}
		}

		void application::onEvent(Event& e)
		{
			if (e.getEventType() == luna::eventType::WindowClose) running = false;
		}
	}
}
