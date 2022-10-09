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

            swapchainImages = mSwapchain.get_images().value();
            swapChainImageViews = mSwapchain.get_image_views().value();
			return VK_SUCCESS;
		}
		VkResult vulkanSwapchain::recreateSwapchain()
		{
            
			return VkResult();
		}
        VkResult vulkanSwapchain::getNextImage(VkSemaphore presentSemaphore,uint32_t* imageIndex)
        {
            
            return vkAcquireNextImageKHR(mSwapchainSpec.device, mSwapchain.swapchain, 1000000000, presentSemaphore, nullptr, imageIndex);

        }
        VkResult vulkanSwapchain::destroySwapchain()
		{
            for (size_t i = 0; i < swapChainImageViews.size(); i++)
            {
                //vkDestroyImage(mSwapchainSpec.device, swapchainImages[i], NULL);
                //vkDestroyImageView(mSwapchainSpec.device, swapChainImageViews[i], NULL);
            }
            vkDestroySwapchainKHR(mSwapchainSpec.device, mSwapchain, nullptr);
			return VK_SUCCESS;
		}



        
        VkPresentModeKHR vulkanSwapchain::chooseSwapPresentMode(const std::vector<VkPresentModeKHR>& availablePresentModes) {
            for (const auto& availablePresentMode : availablePresentModes) 
            {
                if (availablePresentMode == VK_PRESENT_MODE_IMMEDIATE_KHR) { return availablePresentMode; }
                else if (availablePresentMode == VK_PRESENT_MODE_MAILBOX_KHR) { return availablePresentMode; }
            }
            return VK_PRESENT_MODE_FIFO_KHR;
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
        void vulkanSwapchain::createFramebuffers(VkRenderPass renderPass)
        {
            
            for (size_t i = 0; i < swapChainImageViews.size(); i++) 
            {
                VkFramebufferCreateInfo framebufferInfo{};
                framebufferInfo.sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;
                framebufferInfo.renderPass = renderPass;
                framebufferInfo.attachmentCount = 1;
                framebufferInfo.pAttachments = mSwapchain.get_image_views().value().data();
                framebufferInfo.width = mSwapchainSpec.window->getWidth();
                framebufferInfo.height = mSwapchainSpec.window->getHeight();
                framebufferInfo.layers = 1;

                VkFramebuffer frameBuffer;
                vkCreateFramebuffer(mSwapchainSpec.device, &framebufferInfo, nullptr, &frameBuffer);
                swapchainFrameBuffers.push_back(frameBuffer);
            }
        }
	}
}
