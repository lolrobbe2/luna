#include "imGui.h"
#ifdef IMGUI_API
#include <core/debug/debugMacros.h>
#include <backends/imgui_impl_glfw.h>
#include <backends/imgui_impl_vulkan.h>
#include <core/artemis/device/device.h>
#include <backends/imgui_impl_glfw.cpp>
namespace luna 
{
	static ref<artemis::pipeline> p_imguiPipeline;
	static ref<artemis::swapchain>p_swapchain;
	static artemis::descriptorPool descriptorPool;
	static const artemis::device* p_device;
	imGui::imGui(const artemis::device& device,ref<artemis::swapchain> p_swapchain)
	{
		p_device = device;
		luna::p_swapchain = p_swapchain;
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
		io.Fonts->Build();
		LN_ERR_FAIL_COND_MSG(!ImGui_ImplGlfw_InitForVulkan(device, false),"[IMGUI] unable to init IMGUI!");
		
		//ref<vulkan::vulkanPipeline> vPipeline = std::dynamic_pointer_cast<vulkan::vulkanPipeline>(pipeline);

	}
	void imGui::setPipeline(ref<artemis::pipeline> p_pipeline)
	{
		if (p_imguiPipeline) LN_CORE_WARN("p_imguiPipeline was not null: OVERRIDEING!");
		p_imguiPipeline = p_pipeline;
		
	}
	void imGui::setRenderPass(ref<artemis::renderPass> p_renderPass)
	{
		ImGui_ImplVulkan_InitInfo init_info = {};
		init_info.Instance = *p_device;
		init_info.PhysicalDevice = *p_device;
		init_info.Device = *p_device;
		init_info.Queue = p_device->getQueue(vkb::QueueType::graphics);
		init_info.DescriptorPool = descriptorPool;
		init_info.MinImageCount = p_device->surfaceCapabilities().minImageCount + 1;
		init_info.ImageCount = p_swapchain->size();
		init_info.MSAASamples = VK_SAMPLE_COUNT_1_BIT;
		LN_ERR_FAIL_COND_MSG(!ImGui_ImplVulkan_Init(&init_info, *p_renderPass),"[IMGUI] unable to init vulkan impl");

		ref<artemis::commandPool> p_commandPool = p_device->getCommandPool(vkb::QueueType::graphics);
		ref<artemis::commandBuffer> p_commandBuffer = p_commandPool->getCommandBuffer();
		p_commandBuffer->begin(VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT);
		LN_ERR_FAIL_COND_MSG(!ImGui_ImplVulkan_CreateFontsTexture(*p_commandBuffer),"[IMGUI] unable to create fonts texture");
		p_commandBuffer->end();
		p_commandPool->flush({p_commandBuffer.get()}, {},{},nullptr,nullptr,false);
		p_device->waitIdle();
		ImGui_ImplVulkan_DestroyFontUploadObjects();


	}
}
#endif