#pragma once 
#include <core/vulkan/rendering/vulkanPipeline.h>
namespace luna
{
	namespace gui
	{
		class vulkanImgui
		{
		public:
			vulkanImgui(ref<renderer::pipeline> pipeline);
			~vulkanImgui();

		private:
			ref<renderer::pipeline> pipeline;
			VkDescriptorPool imguiPool;
		};
	}
}
