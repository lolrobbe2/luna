#include "vulkanImgui.h"
#include <core/vulkan/device/vulkanDevice.h>	
#include <backends/imgui_impl_glfw.h>
#include <backends/imgui_impl_vulkan.h>
#include <core/vulkan/rendering/vulkanPipeline.h>
namespace luna
{
	namespace gui
	{
		vulkanImgui::vulkanImgui(ref<renderer::pipeline> pipeline)
		{
			LN_PROFILE_FUNCTION();
			ref<renderer::device> device = std::dynamic_pointer_cast<vulkan::vulkanPipeline>(pipeline)->layout.device;
			vkb::Device vDevice = std::dynamic_pointer_cast<vulkan::vulkanDevice>(device)->getDeviceHandles().device;
			this->pipeline = pipeline;
			VkDescriptorPoolSize pool_sizes[] =
			{
				{ VK_DESCRIPTOR_TYPE_SAMPLER, 1000 },
				{ VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, 1000 },
				{ VK_DESCRIPTOR_TYPE_SAMPLED_IMAGE, 1000 },
				{ VK_DESCRIPTOR_TYPE_STORAGE_IMAGE, 1000 },
				{ VK_DESCRIPTOR_TYPE_UNIFORM_TEXEL_BUFFER, 1000 },
				{ VK_DESCRIPTOR_TYPE_STORAGE_TEXEL_BUFFER, 1000 },
				{ VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, 1000 },
				{ VK_DESCRIPTOR_TYPE_STORAGE_BUFFER, 1000 },
				{ VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER_DYNAMIC, 1000 },
				{ VK_DESCRIPTOR_TYPE_STORAGE_BUFFER_DYNAMIC, 1000 },
				{ VK_DESCRIPTOR_TYPE_INPUT_ATTACHMENT, 1000 }
			};

			VkDescriptorPoolCreateInfo pool_info = {};
			pool_info.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO;
			pool_info.flags = VK_DESCRIPTOR_POOL_CREATE_FREE_DESCRIPTOR_SET_BIT;
			pool_info.maxSets = 1000;
			pool_info.poolSizeCount = std::size(pool_sizes);
			pool_info.pPoolSizes = pool_sizes;

			
			LN_CORE_INFO("imgui descriptor pool creation result = {0}" ,vkCreateDescriptorPool(vDevice, &pool_info, nullptr, &imguiPool));
			ImGui::CreateContext();
			ImGuiIO& io = ImGui::GetIO();
			(void)io;
			io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
			io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;
			io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;
			io.IniFilename = "guiConfig.ini";
			io.IniSavingRate = INFINITE;
			LN_CORE_INFO("imgui init for GLFW-vulkan = {0}",ImGui_ImplGlfw_InitForVulkan((GLFWwindow*)device->window->getWindow(), false));
			ImGui_ImplVulkan_InitInfo init_info = {};
			init_info.Instance = std::dynamic_pointer_cast<vulkan::vulkanDevice>(device)->getDeviceHandles().instance;
			init_info.PhysicalDevice = vDevice.physical_device;
			init_info.Device = vDevice;
			init_info.Queue = vDevice.get_queue(vkb::QueueType::present).value();
			init_info.DescriptorPool = imguiPool;
			init_info.MinImageCount = std::dynamic_pointer_cast<vulkan::vulkanDevice>(device)->swapchain->mSwapchain.image_count;
			init_info.ImageCount = std::dynamic_pointer_cast<vulkan::vulkanDevice>(device)->swapchain->mSwapchain.image_count;
			init_info.MSAASamples = VK_SAMPLE_COUNT_1_BIT;
			ref<vulkan::vulkanPipeline> vPipeline = std::dynamic_pointer_cast<vulkan::vulkanPipeline>(pipeline);
			LN_CORE_INFO("imgui init vulkan = {0}",ImGui_ImplVulkan_Init(&init_info,vPipeline->getRenderPass()));

			VkCommandPool commandPool;
			VkCommandPoolCreateInfo poolInfo{};
			poolInfo.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
			poolInfo.queueFamilyIndex = vDevice.get_queue_index(vkb::QueueType::graphics).value();
			poolInfo.flags = 0;
			vkCreateCommandPool(vDevice, &poolInfo, nullptr, &commandPool);

			VkCommandBuffer commandBuffer;
			
			VkCommandBufferAllocateInfo allocateInfo{ VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO };
			allocateInfo.commandBufferCount = 1;
			allocateInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
			allocateInfo.commandPool = commandPool;
			
			vkAllocateCommandBuffers(vDevice, &allocateInfo, &commandBuffer);

			VkCommandBufferBeginInfo info = {};
			info.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
			info.pNext = nullptr;

			info.pInheritanceInfo = nullptr;
			info.flags = VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT;
			vkBeginCommandBuffer(commandBuffer, &info);
			ImGui_ImplVulkan_CreateFontsTexture(commandBuffer);
			vkEndCommandBuffer(commandBuffer);
			VkSubmitInfo submitInfo = {};
			submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
			submitInfo.pNext = nullptr;

			submitInfo.waitSemaphoreCount = 0;
			submitInfo.pWaitSemaphores = nullptr;
			submitInfo.pWaitDstStageMask = nullptr;
			submitInfo.commandBufferCount = 1;
			submitInfo.pCommandBuffers = &commandBuffer;
			submitInfo.signalSemaphoreCount = 0;
			submitInfo.pSignalSemaphores = nullptr;
			vkQueueSubmit(vDevice.get_queue(vkb::QueueType::graphics).value(), 1,&submitInfo , VK_NULL_HANDLE);
			vkDeviceWaitIdle(vDevice);
			ImGui_ImplVulkan_DestroyFontUploadObjects();
			vkFreeCommandBuffers(vDevice, commandPool, 1, &commandBuffer);
			vkDestroyCommandPool(vDevice, commandPool, nullptr);
			
			std::dynamic_pointer_cast<vulkan::vulkanDevice>(device)->swapchain->initViewport(std::dynamic_pointer_cast<vulkan::vulkanPipeline>(pipeline)->maxFramesInFlight);
			LN_CORE_INFO("imgui init complete");
		}
		vulkanImgui::~vulkanImgui()
		{
			LN_PROFILE_FUNCTION();
			ImGui::SaveIniSettingsToDisk("guiConfig.ini");
			ref<renderer::device> device = std::dynamic_pointer_cast<vulkan::vulkanPipeline>(pipeline)->layout.device;
			vkb::Device vDevice = std::dynamic_pointer_cast<vulkan::vulkanDevice>(device)->getDeviceHandles().device;
			std::dynamic_pointer_cast<vulkan::vulkanDevice>(device)->destroyViewport();
			//vkDestroyDescriptorPool(vDevice, imguiPool, nullptr);
			ImGui_ImplVulkan_Shutdown();
		}
	}
}
