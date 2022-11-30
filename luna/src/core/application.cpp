#pragma once
#include <lnpch.h>
#include <core/application.h>
namespace luna
{
	namespace application
	{
		application::application()
		{
			LN_PROFILE_BEGIN_SESSION("luna engine startup", "./debug/luna-profile-startUp.json");
			Log::Init();
			LN_PROFILE_SCOPE("engine startup");
			mWindow = ref<vulkan::window>(vulkan::window::windowCreate());
			mWindow->setEventCallBack(LN_BIND_EVENT_FN(onEvent));
			renderer::renderer::init(mWindow);
			renderer::renderer2D::init();
			LN_PROFILE_END_SESSION();
			
		}
		application::~application()
		{
			LN_PROFILE_BEGIN_SESSION("luna engine shutdown", "./debug/luna-profile-shutdown.json");
			renderer::renderer2D::shutdown();
			LN_PROFILE_END_SESSION();
		}

		void application::run()
		{
			LN_PROFILE_BEGIN_SESSION("luna engine runtime", "./debug/luna-profile-runtime.json");
			while (running)
			{
				
				mWindow->onUpdate();
				LN_PROFILE_SCOPE("drawing");
				renderer::renderer2D::BeginScene();
				renderer::renderer2D::drawQuad({ 0.5f,0.5f ,0.0f }, { 1.0f ,1.0f });
				renderer::renderer2D::drawQuad({ -0.5f,-0.5f ,0.0f }, { 1.0f ,1.0f });
				renderer::renderer2D::endScene();
				renderer::renderer::newFrame();
			}
			LN_PROFILE_END_SESSION();
		}

		void application::onEvent(Event& e)
		{
			if (e.getEventType() == luna::eventType::WindowClose) running = false;
		}
	}
}
