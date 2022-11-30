#pragma once
#include <core/core.h>
#include <core/platform/platform.h>
#include <core/vulkan/window/window.h>
#include <core/events/event.h>
#include <core/rendering/renderer2D.h>
#include <core/utils/objectStorage.h>
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
		private:
			ref<vulkan::window> mWindow;
			utils::objectStorage<glm::mat4> windowStorage;
			bool running = true;
		};
		/**
		 * @brief creates the application and returns a pointer to the appliccation.
		 */
		application* createApplication();
	}
}

