#include "imGui.h"
#ifdef IMGUI_API
#include <backends/imgui_impl_glfw.h>
#include <backends/imgui_impl_vulkan.h>
namespace luna 
{
	imGui::imGui(const artemis::device& device,ref<artemis::swapchain> p_swapchain)
	{
		this->p_device = device;
		artemis::descriptorPoolBuilder poolBuilder = p_device->getDescriptorPoolBuilder(nullptr);
		poolBuilder.setSamplerAmount(1000)
			.setCombinedImageSamplerAmount(1000)
			.setSampledImageAmount(1000)
			.setStorageImageAmount(1000)
			.setUniformTexelBuffer(1000)
			.setStorageTexelBuffer(1000)
			.setUniformBufferAmount(1000)
			.setStorageBufferAmount(1000)
			.setUniformBufferDynamicAmount(1000)
			.setStorageBufferDynamicAmount(1000)
			.setInputAttachementAmount(1000);

		descriptorPool = poolBuilder.build();

		ImGui::CreateContext();
		ImGuiIO& io = ImGui::GetIO();
		(void)io;
		io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
		io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;
		io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;
		io.IniFilename = "guiConfig.ini";
		io.IniSavingRate = INFINITE;
		ImGui_ImplGlfw_InitForVulkan(device, false);
		ImGui_ImplVulkan_InitInfo init_info = {};
		init_info.Instance = device;
		init_info.PhysicalDevice = device;
		init_info.Device = device;
		init_info.Queue = device.getQueue(vkb::QueueType::graphics);
		init_info.DescriptorPool = descriptorPool;
		init_info.MinImageCount = device.surfaceCapabilities().minImageCount;
		init_info.ImageCount = p_swapchain->size();
		init_info.MSAASamples = VK_SAMPLE_COUNT_1_BIT;
		//ref<vulkan::vulkanPipeline> vPipeline = std::dynamic_pointer_cast<vulkan::vulkanPipeline>(pipeline);

	}
	void imGui::setPipeline(ref<artemis::pipeline> p_pipeline)
	{
		if (p_imguiPipeline) LN_CORE_WARN("p_imguiPipeline was not null: OVERRIDEING!");
		p_imguiPipeline = p_pipeline;
	}
}
#endif