#pragma once 
#include <core/vulkan/rendering/vulkanPipeline.h>
#ifndef DISABLE_IMGUI

namespace luna
{
	namespace gui
	{
		class vulkanImgui
		{
		public:
			/**
			 * @brief initializes imgui for vulkan.
			 * 
			 * \param ref<renderer::pipeline> pipeline reference to the vulkan pipeline in base class form
			 */
			vulkanImgui(ref<renderer::pipeline> pipeline);
			~vulkanImgui();

		private:
			ref<renderer::pipeline> pipeline; 
			VkDescriptorPool imguiPool;
		};
	}
}

#endif // DISABLE_IMGUI