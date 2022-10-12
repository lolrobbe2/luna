#include "vulkanImgui.h"
#include <core/vulkan/device/vulkanDevice.h>	
#include <backends/imgui_impl_glfw.h>
#include <backends/imgui_impl_vulkan.h>
namespace luna
{
	namespace gui
	{
		vulkanImgui::vulkanImgui(ref<renderer::pipeline> pipeline)
		{
			ref<renderer::device> device = std::dynamic_pointer_cast<vulkan::vulkanPipeline>(pipeline)->layout.device;
			vkb::Device vDevice = std::dynamic_pointer_cast<vulkan::vulkanDevice>(device)->getDeviceHandles().device;
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

			
			LN_CORE_INFO("imgui descriptor set creation result = {0}" ,vkCreateDescriptorPool(vDevice, &pool_info, nullptr, &imguiPool));
			ImGui::CreateContext();
			ImGui_ImplGlfw_InitForVulkan((GLFWwindow*)device->window->getWindow(), 0);

			ImGui_ImplVulkan_InitInfo init_info = {};
			init_info.Instance = std::dynamic_pointer_cast<vulkan::vulkanDevice>(device)->getDeviceHandles().instance;
			init_info.PhysicalDevice = vDevice.physical_device;
			init_info.Device = vDevice;
			init_info.Queue = vDevice.get_queue(vkb::QueueType::present).value();
			init_info.DescriptorPool = imguiPool;
			init_info.MinImageCount = 3;
			init_info.ImageCount = 3;
			init_info.MSAASamples = VK_SAMPLE_COUNT_1_BIT;
			VkCommandPool commandPool;
			VkCommandPoolCreateInfo poolInfo{};
			poolInfo.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
			poolInfo.queueFamilyIndex = vDevice.get_queue_index(vkb::QueueType::present).value();
			poolInfo.flags = VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT;
			vkCreateCommandPool(vDevice, &poolInfo, nullptr, &commandPool);
		}
	}
}
