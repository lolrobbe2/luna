#pragma once

#ifndef IMGUI_DISABLE
#ifndef _IMGUI_
#define _IMGUI_
#include <core/core.h>
#include <imgui.h>
namespace luna
{
	namespace artemis
	{
		class device;
		class swapchain;
		class pipeline;
	}
	/// <summary>
	/// rendering interface for imgui
	/// </summary>
	class imGui
	{
	public:
		imGui(const artemis::device& device,ref<artemis::swapchain> p_swapchain);
		void setPipeline(ref<artemis::pipeline> p_pipeline);
	private:

		
	};
} 
#endif // !_IMGUI_
#endif // !IMGUI_DISABLE
