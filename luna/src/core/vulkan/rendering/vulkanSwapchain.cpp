#include "vulkanSwapchain.h"
namespace luna
{
	namespace vulkan
	{
		vulkanSwapchain::vulkanSwapchain(const swapchainSpec& swapChainSpec)
		{
			LN_CORE_TRACE("swapchain creation result = {0}",createSwapchain(swapChainSpec));
		}
		vulkanSwapchain::~vulkanSwapchain()
		{
			destroySwapchain();
		}
		VkResult vulkanSwapchain::createSwapchain(const swapchainSpec& swapChainSpec)
		{

            mSwapchainSpec = swapChainSpec;
            vkb::SwapchainBuilder swapchainBuilder{ swapChainSpec.physicalDevice, swapChainSpec.device, swapChainSpec.surface };
            
            mSwapchain = swapchainBuilder
                .use_default_format_selection()
                //use vsync present mode
                .set_desired_present_mode(VK_PRESENT_MODE_FIFO_KHR)
                .set_desired_extent(swapChainSpec.window->getWidth(), swapChainSpec.window->getHeight())
                .build()
                .value();
			return VK_SUCCESS;
		}
		VkResult vulkanSwapchain::recreateSwapchain()
		{
            vkDeviceWaitIdle(mSwapchainSpec.device);
            
            for (size_t i = 0; i < frameBuffers.size(); i++)
            {
                vkDestroyFramebuffer(mSwapchain.device, frameBuffers[i], nullptr);
            }

            vkb::SwapchainBuilder swapchainBuilder{ mSwapchainSpec.physicalDevice, mSwapchainSpec.device, mSwapchainSpec.surface };
            auto newSwapchain = swapchainBuilder.set_old_swapchain(mSwapchain)
                .use_default_format_selection()
                //use vsync present mode
                .set_desired_present_mode(VK_PRESENT_MODE_FIFO_KHR)
                .set_desired_extent(mSwapchainSpec.window->getWidth(), mSwapchainSpec.window->getHeight())
                .set_required_min_image_count(mSwapchain.image_count)
                .build();
              vkb::destroy_swapchain(mSwapchain);
              mSwapchain = newSwapchain.value();
			return VK_SUCCESS;
		}
		VkResult vulkanSwapchain::destroySwapchain()
		{
            for (size_t i = 0; i < frameBuffers.size(); i++)
            {               
                vkDestroyFramebuffer(mSwapchain.device, frameBuffers[i], nullptr);
            }
            mSwapchain.destroy_image_views(mSwapchain.get_image_views().value());
            vkDestroySwapchainKHR(mSwapchainSpec.device, mSwapchain, nullptr);
			return VK_SUCCESS;
		}

        VkViewport vulkanSwapchain::getViewport()
        {
            VkViewport viewport;
            viewport.x = 0.0f;
            viewport.y = 0.0f;
            viewport.width = (float)mSwapchainSpec.window->getWidth();
            viewport.height = (float)mSwapchainSpec.window->getHeight();
            viewport.minDepth = 0.0f;
            viewport.maxDepth = 1.0f;
            return viewport;
        }

        VkRect2D vulkanSwapchain::getScissor()
        {
            VkRect2D scissor;
            scissor.extent = { mSwapchainSpec.window->getWidth(),mSwapchainSpec.window->getHeight() };
            scissor.offset = { 0,0 };
            return scissor;
        }
               
	}
}
