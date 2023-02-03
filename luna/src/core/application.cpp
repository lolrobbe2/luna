#pragma once
#include <lnpch.h>
#include <core/application.h>
namespace luna
{
	namespace application
	{
		application::application()
		{
			//TODO images with less than 3 channels RGB 

			LN_PROFILE_BEGIN_SESSION("luna engine startup", "./debug/luna-profile-startUp.json");
			Log::Init();
			LN_PROFILE_SCOPE("engine startup");
			mWindow = ref<vulkan::window>(vulkan::window::windowCreate());
			mWindow->setEventCallBack(LN_BIND_EVENT_FN(onEvent));
			renderer::renderer::init(mWindow);
			renderer::renderer2D::init();
			logicGatesImage = renderer::texture::create("./src/assets/media/logic_gates.png");
			statueImage = renderer::texture::create("./src/assets/media/test.png");
			upgradeImage = renderer::texture::create("./src/assets/media/geheugenUpgrade.png");
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

				renderer::renderer2D::drawQuad({ 0.75f,0.75f ,0.0f }, { 1.0f ,1.0f }, statueImage);
				renderer::renderer2D::drawQuad({ -0.75f,-0.75f ,0.0f }, { 1.0f ,1.0f }, logicGatesImage);
				renderer::renderer2D::drawQuad({ 0.75f,-0.75f ,0.0f }, { 1.0f ,1.0f }, upgradeImage);

				//renderer::renderer2D::drawQuad({ 0.5f,0.5f ,0.0f }, { 1.0f ,1.0f }, { 246.0f , 83.0f , 20.0f,1.0f });
				//renderer::renderer2D::drawQuad({ -0.5f,-0.5f ,0.0f }, { 1.0f ,1.0f }, { 241.0f, 161.0f,  0.0f,1.0f });
				//renderer::renderer2D::drawQuad({ 0.5f,-0.5f ,0.0f }, { 1.0f ,1.0f },{ 124.0f, 187.0f, 0.0f,1.0f });
				//renderer::renderer2D::drawQuad({ -0.5f,0.5f ,0.0f }, { 1.0f ,1.0f }, { 255.0f, 187.0f, 0.0f,1.0f });
				renderer::renderer2D::endScene();
				if (ImGui::Begin("settings"))
				{
					ImGui::Text(("framerate = " + std::to_string(ImGui::GetIO().Framerate) + " FPS").c_str());
					ImGui::Text(("frameTime = " + std::to_string(ImGui::GetIO().DeltaTime * 1000) + " ms").c_str());
					ImGui::Text(("drawCalls = " + std::to_string(renderer::renderer2D::getStats().drawCalls)).c_str());
					ImGui::Text(("quadCount = " + std::to_string(renderer::renderer2D::getStats().quadCount)).c_str());
					ImGui::Text(("vertexCount = " + std::to_string(renderer::renderer2D::getStats().getTotalVertexCount())).c_str());
					ImGui::Text(("indexCount = " + std::to_string(renderer::renderer2D::getStats().getTotalIndexCount())).c_str());
				}
				ImGui::End();
				renderer::renderer::newFrame();
			}
			LN_PROFILE_END_SESSION();
		}

		void application::onEvent(Event& e)
		{
			if (e.getEventType() == luna::eventType::WindowClose) running = false;
			else if (e.getEventType() == luna::eventType::WindowLostFocus) LN_CORE_INFO("lost focus!");
			else if (e.getEventType() == luna::eventType::WindowFocus) LN_CORE_INFO("gained focus!");
		}
	}
}
