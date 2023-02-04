#pragma once
#include <core/core.h>
#include <core/platform/platform.h>
#include <core/vulkan/window/window.h>
#include <core/events/event.h>
#include <core/events/applicationEvent.h>
#include <core/rendering/renderer2D.h>
#include <core/utils/objectStorage.h>
#include <core/utils/layerStack.h>
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

			bool onWindowClose(windowCloseEvent& e);
			bool onWindowResize(windowResizeEvent& e);
			/**
			 * @brief pushes a layer to the layerstack.
			 * 
			 * \param utils::layer* layer  (layer to be pushed to the stack)
			 */
			void pushLayer(utils::layer* layer);
			void pushOverlay(utils::layer* layer);
		private:
			ref<vulkan::window> mWindow;
			ref<renderer::texture> logicGatesImage;
			ref<renderer::texture> statueImage;
			ref<renderer::texture> upgradeImage;
			utils::layerStack layerStack;
			float lastFrameTime = 0.0f;
			bool running = true;
			bool minimized = false;
		};
		/**
		 * @brief creates the application and returns a pointer to the appliccation.
		 */
		application* createApplication();
	}
}

