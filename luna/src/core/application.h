#pragma once
#include <core/core.h>
#include <core/platform/platform.h>
#include <core/vulkan/window/window.h>
#include <core/events/event.h>
#include <core/events/applicationEvent.h>
#include <core/rendering/renderer2D.h>
#include <core/utils/objectStorage.h>
#include <core/utils/layerStack.h>
#include <core/scene/scene.h>


int main(int argc, char** argv);

namespace luna
{
	namespace application
	{
		class LN_API application
		{
		public:
			/**
			 * @brief application constructor, initializes the application and engine.
			 * 
			 */
			application();
			virtual ~application();
			/**
			 * @brief the main loop of the application.
			 * 
			 */
			void run();
			/**
			 * @brief the main event callback of the application.
			 * 
			 * \param e
			 */
			void onEvent(Event& e);
			vulkan::window& getWindow() { return *mWindow; }
			bool onWindowClose(windowCloseEvent& e);
			bool onWindowResize(windowResizeEvent& e);
			/**
			 * @brief pushes a layer to the layerstack.
			 * 
			 * \param utils::layer* layer  (layer to be pushed to the stack)
			 */
			void pushLayer(utils::layer* layer);
			void pushOverlay(utils::layer* layer);

			static application& get();
		private:
			friend class os;
			ref<vulkan::window> mWindow;
			ref<renderer::texture> logicGatesImage;
			ref<renderer::texture> statueImage;
			ref<renderer::texture> upgradeImage;
			scene scene;
			utils::layerStack layerStack;
			float lastFrameTime = 0.0f;
			bool running = true;
			bool minimized = false;
		private:

			friend int ::main(int argc, char** argv);
		};
		/**
		 * @brief creates the application and returns a pointer to the appliccation.
		 */
		application* createApplication();
	}
}

