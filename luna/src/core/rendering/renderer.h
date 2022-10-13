#pragma once
#include <core/vulkan/window/window.h>
#include <core/rendering/device.h>
#include <core/rendering/pipeline.h>
#include <core/vulkan/gui/vulkanImgui.h> //TODO make base gui header
namespace luna
{
	namespace renderer
	{
		class renderer
		{
		public:
			/**
			 * @brief the main renderer class.
			 * 
			 * \param window a ref to the main window
			 */
			static void init(const ref<vulkan::window>& window);
			/**
			 * @brief create a frame in the render pipline based on the given input.
			 * 
			 */
			static void createFrame();
			/**
			 * @brief renders a new frame.
			 * 
			 */
			static void newFrame();
		private:
			inline static ref<device> rendererDevice; //the main renderDevice (GPU handle)
			inline static ref<pipeline> rendererPipeline;// the main graphics pipeline
			inline static renderer::pipelineLayout layout; //the main graphics pipline layout
			inline static ref<gui::vulkanImgui> gui;
			inline static bool Selected;
		};

	}
}
