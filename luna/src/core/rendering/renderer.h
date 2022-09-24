#pragma once
#include <core/vulkan/window/window.h>
#include <core/rendering/device.h>
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
