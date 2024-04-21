#pragma once
#include <imgui.h>
#include <core/artemis/device/device.h>
#ifdef IMGUI_API
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
#endif