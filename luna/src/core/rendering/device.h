#pragma once
#include <core/vulkan/window/window.h>


namespace luna
{
	namespace renderer
	{
		class device
		{
		public:
			//device(ref<vulkan::window>& window);
			virtual ~device() {};
			virtual void createContext() = 0;
			virtual void destroyContext()  = 0;
		private:
			ref<vulkan::window> window;
		};
	}
}
