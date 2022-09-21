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
			swapChainSupportDetails swapChainSupport = querySwapChainSupport(mSwapchainSpec.physicalDevice);
			VkSurfaceFormatKHR surfaceFormat = chooseSwapSurfaceFormat(swapChainSupport.formats);
			VkPresentModeKHR presentMode = chooseSwapPresentMode(swapChainSupport.presentModes);
			mSwapchainSpec.swapchainExtent = chooseSwapExtent(swapChainSupport.capabilities);
            uint32_t imageCount = swapChainSupport.capabilities.minImageCount + 1;
            if (swapChainSupport.capabilities.maxImageCount > 0 && imageCount > swapChainSupport.capabilities.maxImageCount) {imageCount = swapChainSupport.capabilities.maxImageCount;}
            VkSwapchainCreateInfoKHR createInfo{};
            createInfo.sType = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR;
            createInfo.surface = mSwapchainSpec.surface;

            createInfo.minImageCount = imageCount;
            createInfo.imageFormat = surfaceFormat.format;
            createInfo.imageColorSpace = surfaceFormat.colorSpace;
            createInfo.imageExtent = mSwapchainSpec.swapchainExtent;
            createInfo.imageArrayLayers = 1;
            createInfo.imageUsage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT | VK_IMAGE_USAGE_TRANSFER_DST_BIT;


            uint32_t queueFamilyIndices[] = { mSwapchainSpec.indices.graphicsFamily.value(), mSwapchainSpec.indices.presentFamily.value() };

            if (mSwapchainSpec.indices.graphicsFamily != mSwapchainSpec.indices.presentFamily) 
            {
                createInfo.imageSharingMode = VK_SHARING_MODE_CONCURRENT;
                createInfo.queueFamilyIndexCount = 2;
                createInfo.pQueueFamilyIndices = queueFamilyIndices;
            }
            else 
            {
                createInfo.imageSharingMode = VK_SHARING_MODE_EXCLUSIVE;
                createInfo.queueFamilyIndexCount = 0; // Optional
                createInfo.pQueueFamilyIndices = nullptr; // Optional
            }
            createInfo.preTransform = swapChainSupport.capabilities.currentTransform;
            createInfo.compositeAlpha = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR;

            createInfo.presentMode = presentMode;
            createInfo.clipped = VK_TRUE;
            createInfo.oldSwapchain = VK_NULL_HANDLE;
            VkResult result = vkCreateSwapchainKHR(mSwapchainSpec.device, &createInfo, nullptr, &mSwapchain);
            vkGetSwapchainImagesKHR(mSwapchainSpec.device, mSwapchain, &imageCount, nullptr);
            swapchainImages.resize(imageCount);
            vkGetSwapchainImagesKHR(mSwapchainSpec.device, mSwapchain, &imageCount, swapchainImages.data());
            swapchainImageFormat = surfaceFormat.format;
            createImageViews();
			return result;
		}
		VkResult vulkanSwapchain::recreateSwapchain()
		{
			return VkResult();
		}
		VkResult vulkanSwapchain::destroySwapchain()
		{
            for (size_t i = 0; i < swapChainImageViews.size(); i++)
            {
                //vkDestroyImage(mSwapchainSpec.device, swapchainImages[i], NULL);
                //vkDestroyImageView(mSwapchainSpec.device, swapChainImageViews[i], NULL);
            }
            vkDestroySwapchainKHR(mSwapchainSpec.device, mSwapchain, nullptr);
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
            for (const auto& availableFormat : availableFormats) {if (availableFormat.format == VK_FORMAT_R8G8B8A8_SRGB && availableFormat.colorSpace == VK_COLOR_SPACE_SRGB_NONLINEAR_KHR) return availableFormat;}
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
        void vulkanSwapchain::createImageViews() 
        {
            swapChainImageViews.clear();
            swapChainImageViews.resize(swapchainImages.size());
            for (uint32_t i = 0; i < swapchainImages.size(); i++) {swapChainImageViews[i] = createImageView(swapchainImages[i], swapchainImageFormat);}
        }

        VkImageView vulkanSwapchain::createImageView(VkImage image, VkFormat format)
        {
            VkImageViewCreateInfo viewInfo{};
            viewInfo.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
            viewInfo.image = image;
            viewInfo.viewType = VK_IMAGE_VIEW_TYPE_2D;
            viewInfo.format = format;
            viewInfo.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
            viewInfo.subresourceRange.baseMipLevel = 0;
            viewInfo.subresourceRange.levelCount = 1;
            viewInfo.subresourceRange.baseArrayLayer = 0;
            viewInfo.subresourceRange.layerCount = 1;

            VkImageView imageView;
            if (vkCreateImageView(mSwapchainSpec.device, &viewInfo, nullptr, &imageView) != VK_SUCCESS) {
                throw std::runtime_error("failed to create texture image view!");
            }

            return imageView;
        }
	}
}
