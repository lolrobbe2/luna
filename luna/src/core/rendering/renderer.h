#pragma once
#include <core/vulkan/device/vulkanDevice.h>
namespace luna
{
	namespace renderer
	{
		class renderer
		{
		public:
			static void init(const ref<vulkan::window>& window);
			static void newFrame();
		private:
			inline static ref<device> rendererDevice;
		};

	}
}
