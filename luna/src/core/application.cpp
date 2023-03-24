#pragma once
#include <lnpch.h>
#include <core/application.h>
#include <core/scene/scene.h>
#include <core/object/classRegister.h>
namespace luna
{
	namespace application
	{
		static application* instance;
		application::application()
		{
			//TODO images with less than 3 channels RGB 

			LN_PROFILE_BEGIN_SESSION("luna engine startup", "./debug/luna-profile-startUp.json");
			instance = this;
			Log::Init();
			LN_PROFILE_SCOPE("engine startup");
			mWindow = ref<vulkan::window>(vulkan::window::windowCreate());
			mWindow->setEventCallBack(LN_BIND_EVENT_FN(onEvent));
			renderer::renderer::init(mWindow);
			renderer::renderer2D::init();
			nodes::classRegister::registerClasses();
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
				float time = glfwGetTime();
				utils::timestep timestep = time - lastFrameTime;
				lastFrameTime = time;
				if (!minimized)
				{
					renderer::renderer2D::BeginScene();
					{
						LN_PROFILE_SCOPE("LayerStack OnUpdate");

						for (utils::layer* layer : layerStack)
							layer->onUpdate(timestep);
					}
					renderer::renderer2D::endScene();
					{
						LN_PROFILE_SCOPE("LayerStack OnImGuiRender");

						for (utils::layer* layer : layerStack)
							layer->onImGuiRender();
					}
					renderer::renderer::newFrame();
				}
				
			}
			LN_PROFILE_END_SESSION();
		}

		void application::onEvent(Event& e)
		{
			eventDispatcher dispatcher(e);
			dispatcher.dispatch<windowCloseEvent>(LN_BIND_EVENT_FN(application::onWindowClose));
			dispatcher.dispatch<windowResizeEvent>(LN_BIND_EVENT_FN(application::onWindowResize));
			for (auto it = layerStack.rbegin(); it != layerStack.rend(); ++it)
			{
				if (e.Handled)
					break;
				(*it)->onEvent(e);
			}
		}

		bool application::onWindowClose(windowCloseEvent& e)
		{
			running = false;
			return true;
		}

		bool application::onWindowResize(windowResizeEvent& e)
		{
			LN_PROFILE_FUNCTION();

			if (e.getWidth() == 0 || e.getHeight() == 0)
			{
				minimized = true;
				return false;
			}

			minimized = false;
			return false;
		}
		void application::pushLayer(utils::layer* layer)
		{
			LN_PROFILE_FUNCTION();

			layerStack.pushLayer(layer);
			layer->onAttach();
		}

		void application::pushOverlay(utils::layer* layer)
		{
			LN_PROFILE_FUNCTION();

			layerStack.pushOverlay(layer);
			layer->onAttach();
		}
		application& application::get()
		{
			return *instance;
		}
	}
}
