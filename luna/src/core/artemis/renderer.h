#pragma once
#include <core/artemis/device/device.h>
namespace luna 
{
	namespace artemis 
	{
		class renderer
		{
		public:
			renderer(const ref<vulkan::window>& window);
		private:
			device c_device;
			ref<swapchain> p_swapChain;
			ref<commandPool> p_graphicsCommandPool;
			ref<commandPool> p_transferCommandPool;
			ref<renderPass> p_renderPass;
		};
	}
}


