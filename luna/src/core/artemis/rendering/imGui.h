#pragma once
#ifndef IMGUI_DISABLE
#ifndef _IMGUI_
#define _IMGUI_
#include <imgui.h>
#include <backends/imgui_impl_glfw.h>
#include <core/artemis/device/device.h>
namespace luna
{
	/// <summary>
	/// rendering interface for imgui
	/// </summary>
	class imGui
	{
	public:
		imGui(const artemis::device& device,ref<artemis::swapchain> p_swapchain);
		void setPipeline(ref<artemis::pipeline> p_pipeline);
	private:
		ref<artemis::pipeline> p_imguiPipeline;
		artemis::descriptorPool& descriptorPool = *new artemis::descriptorPool();
		
		const artemis::device* p_device;
		
	};
} 
#endif // !_IMGUI_
#endif // !IMGUI_DISABLE
