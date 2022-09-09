#include "vulkanSwapchain.h"
namespace luna
{
	namespace renderer
	{
		vulkanSwapchain::vulkanSwapchain(const swapchainSpec& swapChainSpec)
		{
			createSwapchain(swapChainSpec);
		}
		vulkanSwapchain::~vulkanSwapchain()
		{
			destroySwapchain();
		}
		VkResult vulkanSwapchain::createSwapchain(const swapchainSpec& swapChainSpec)
		{
			mSwapchainSpec = swapChainSpec;
			swapChainSupportDetails swapChainSupport = querySwapChainSupport(mSwapchainSpec.physicalDevice);
			VkSurfaceFormatKHR surfaceFormat = chooseSwapSurfaceFormat(swapChainSupport.formats);
			VkPresentModeKHR presentMode = chooseSwapPresentMode(swapChainSupport.presentModes);
			mSwapchainSpec.swapchainExtent = chooseSwapExtent(swapChainSupport.capabilities);
			return VkResult();
		}
		VkResult vulkanSwapchain::recreateSwapchain()
		{
			return VkResult();
		}
		VkResult vulkanSwapchain::destroySwapchain()
		{
			return VkResult();
		}



        swapChainSupportDetails vulkanSwapchain::querySwapChainSupport(const VkPhysicalDevice& device) {
            swapChainSupportDetails details;
            vkGetPhysicalDeviceSurfaceCapabilitiesKHR(device, mSwapchainSpec.surface, &details.capabilities);

            uint32_t formatCount;
            vkGetPhysicalDeviceSurfaceFormatsKHR(device, mSwapchainSpec.surface, &formatCount, nullptr);

            if (formatCount != 0) 
            {
                details.formats.resize(formatCount);
                vkGetPhysicalDeviceSurfaceFormatsKHR(device, mSwapchainSpec.surface, &formatCount, details.formats.data());
            }
            return details;
        }

        VkSurfaceFormatKHR vulkanSwapchain::chooseSwapSurfaceFormat(const std::vector<VkSurfaceFormatKHR>& availableFormats)
        {
            for (const auto& availableFormat : availableFormats) {if (availableFormat.format == supportedFormatFamily.getFormatByChannel(4) && availableFormat.colorSpace == VK_COLOR_SPACE_SRGB_NONLINEAR_KHR) return availableFormat;}
            return availableFormats[0];
        }

        VkPresentModeKHR vulkanSwapchain::chooseSwapPresentMode(const std::vector<VkPresentModeKHR>& availablePresentModes) {
            for (const auto& availablePresentMode : availablePresentModes) 
            {
                if (availablePresentMode == VK_PRESENT_MODE_IMMEDIATE_KHR) { return availablePresentMode; }
                else if (availablePresentMode == VK_PRESENT_MODE_MAILBOX_KHR) { return availablePresentMode; }
            }
            return VK_PRESENT_MODE_FIFO_KHR;
        }

        VkExtent2D vulkanSwapchain::chooseSwapExtent(const VkSurfaceCapabilitiesKHR& capabilities)
        {
            if (capabilities.currentExtent.width != UINT32_MAX) {return capabilities.currentExtent;}
            else {
                int width, height;
                glfwGetFramebufferSize((GLFWwindow*)mSwapchainSpec.window->getWindow(), &width, &height);

                VkExtent2D actualExtent = {
                    static_cast<uint32_t>(width),
                    static_cast<uint32_t>(height)
                };

                actualExtent.width = std::max(capabilities.minImageExtent.width, std::min(capabilities.maxImageExtent.width, actualExtent.width));
                actualExtent.height = std::max(capabilities.minImageExtent.height, std::min(capabilities.maxImageExtent.height, actualExtent.height));

                return actualExtent;
            }
        }
	}
}
