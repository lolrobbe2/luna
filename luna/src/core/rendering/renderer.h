#pragma once
#include <core/vulkan/window/window.h>
#include <core/rendering/device.h>
#include <core/rendering/pipeline.h>
namespace luna
{
	namespace renderer
	{
		class renderer
		{
		public:
			static void init(const ref<vulkan::window>& window);
			static void createFrame();
			static void newFrame();
		private:
			inline static ref<device> rendererDevice;
			inline static ref<pipeline> rendererPipeline;
			inline static renderer::pipelineLayout layout;
		};

	}
}
