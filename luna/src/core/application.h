#pragma once
#include <core/core.h>
#include <core/platform/platform.h>
#include <core/vulkan/window/window.h>
#include <core/events/event.h>
namespace luna
{
	namespace application
	{
		class LN_API application
		{
		public:
			application();
			virtual ~application();
			void run();
			void onEvent(Event& e);
		private:
			ref<vulkan::window> mWindow;
		};
		application* createApplication();
	}
}

