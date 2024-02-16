#pragma once
#include <lnpch.h>
#include <core/application.h>
#include <core/scene/scene.h>
#include <core/object/classRegister.h>
#include <core/scripting/scriptingEngine.h>
#include <core/object/methodDB.h>
#include <core/assets/assetManager.h>
#include <core/platform/platformUtils.h>
#include <core/networking/socket.h>
namespace luna
{
	namespace application
	{
		static application* instance;
		application::application()
		{ 
			LN_PROFILE_BEGIN_SESSION("luna engine startup", "./debug/luna-profile-startUp.json");
			platform::os::setCursorShape(platform::APP_STARTING);


			instance = this;
			Log::Init();
			initCore();
			//auto commandPool = device.getCommandPool(vkb::QueueType::graphics, 0);
			//auto commandBuffer = commandPool->getCommandBuffer();
			LN_CORE_INFO("started vulkan device");
			LN_PROFILE_END_SESSION();

			run();
			/*required valid asset manager!*/
			/*
			renderer::renderer::init(mWindow);
			renderer::renderer2D::init();
			nodes::classRegister::registerClasses();
			scripting::scriptingEngine::init();
			methodDB::init();
			networking::Ip::init();
			*/
			platform::os::setCursorShape(platform::ARROW);
			
		}
		application::~application()
		{
			LN_PROFILE_BEGIN_SESSION("luna engine shutdown", "./debug/luna-profile-shutdown.json");
			layerStack.~layerStack();
			platform::os::setCursorShape(platform::ARROW);
			
			/*
			networking::Ip::shutdown();
			networking::netSocket::terminate();
			renderer::renderer2D::shutdown();
			scripting::scriptingEngine::shutdown();
			*/
			mWindow->~window();
			LN_PROFILE_END_SESSION();
		}

		void application::run()
		{
			LN_PROFILE_BEGIN_SESSION("luna engine runtime", "./debug/luna-profile-runtime.json");
			running = true;
			while (running)
			{

				mWindow->onUpdate();
				LN_PROFILE_SCOPE("drawing");
				double time = glfwGetTime();
				utils::timestep timestep = time - lastFrameTime;
				lastFrameTime = time;
				
				executeMainThreadQueue();
				
				if (!minimized)
				{
					p_renderer->beginScene();
					{
						LN_PROFILE_SCOPE("LayerStack OnUpdate");

						for (utils::layer* layer : layerStack)
							layer->onUpdate(timestep);
					}
					p_renderer->endScene();
					{
						LN_PROFILE_SCOPE("LayerStack OnImGuiRender");

						for (utils::layer* layer : layerStack)
							layer->onImGuiRender();
					}
					p_renderer->update();
				}
			}
			LN_PROFILE_END_SESSION();
		}

		void application::initCore()
		{
			LN_PROFILE_SCOPE("engine startup");

			mWindow = ref<vulkan::window>(vulkan::window::windowCreate());
			//mWindow->setEventCallBack(LN_BIND_EVENT_FN(onEvent));
			assets::assetManager::init(true);
			p_renderer = createScope<artemis::renderer>(mWindow);
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

		void application::submitToMainThread(const std::function<void()>& function)
		{
			std::scoped_lock<std::mutex> lock(mainThreadQueueMutex);

			mainThreadQueue.emplace_back(function);
		}

		void application::executeMainThreadQueue()
		{
			std::scoped_lock<std::mutex> lock(mainThreadQueueMutex);

			for (auto& func : mainThreadQueue)
				func();

			mainThreadQueue.clear();
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
		void application::popLayer(utils::layer* layer)
		{
			LN_PROFILE_FUNCTION();

			layerStack.popLayer(layer);
			layer->onDetach();
		}
		application& application::get()
		{
			return *instance;
		}
	}
}
