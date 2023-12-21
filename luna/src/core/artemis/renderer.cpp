#include "renderer.h"
namespace luna 
{
	namespace artemis 
	{
		renderer::renderer(const ref<vulkan::window>& window)
		{
			c_device = device(window);
			p_swapChain = c_device.getSwapchain();
			p_graphicsCommandPool = c_device.getCommandPool(vkb::QueueType::graphics, 0);
			p_transferCommandPool = c_device.getCommandPool(vkb::QueueType::transfer, 0);
			subPassBuilder subPassBuilder;
			VkSubpassDescription subPassDescriptionInfo = subPassBuilder
				.addColorAttachement(VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL)
				.setBindPoint(VK_PIPELINE_BIND_POINT_GRAPHICS)
				.build();
			renderPassBuilder renderPassBuilder(c_device);
			renderPassBuilder.addSubPass(subPassDescriptionInfo).addSubPassDependency;
		}
	}
}