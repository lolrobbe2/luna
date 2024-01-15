#pragma once
#include <core/core.h>
#include <core/platform/platform.h>
#include <core/events/event.h>
#include <core/events/applicationEvent.h>
#include <core/utils/objectStorage.h>
#include <core/utils/layerStack.h>
#include <core/scene/scene.h>
#include <core/artemis/renderer.h>



int main(int argc, char** argv);

namespace luna
{
	namespace application
	{
		class  application
		{
		public:
			/**
			 * @brief application constructor, initializes the application and engine.
			 * 
			 */
			LN_API application();
			LN_API virtual ~application();
			/**
			 * @brief the main loop of the application.
			 * 
			 */
			LN_API void run();
			void initCore();
			/**
			 * @brief the main event callback of the application.
			 * 
			 * \param e
			 */
			void onEvent(Event& e);
			vulkan::window& getWindow() { return *mWindow; }
			bool onWindowClose(windowCloseEvent& e);
			bool onWindowResize(windowResizeEvent& e);
			void submitToMainThread(const std::function<void()>& function);
			void executeMainThreadQueue();
			/**
			 * @brief pushes a layer to the layerstack.
			 * 
			 * \param utils::layer* layer  (layer to be pushed to the stack)
			 */
			LN_API void pushLayer(utils::layer* layer);
			LN_API void pushOverlay(utils::layer* layer);
			LN_API void popLayer(utils::layer* layer);
			static application& get();
		private:
			friend class os;
			ref<vulkan::window> mWindow;
			scene scene;
			utils::layerStack layerStack;
			double lastFrameTime = 0.0f;
			bool running = true;
			bool minimized = false;

			std::vector<std::function<void()>> mainThreadQueue;
			std::mutex mainThreadQueueMutex;
		private:

			friend int ::main(int argc, char** argv);
		};
		/**
		 * @brief creates the application and returns a pointer to the appliccation.
		 */
		LN_API application* createApplication();
	}
}

