#pragma once
#include <core/vulkan/window/window.h>
namespace luna
{
	namespace renderer
	{
		struct swapchainSpec
		{
			VkPhysicalDevice physicalDevice;
			std::shared_ptr<vulkan::window> window = nullptr;
			VkExtent2D swapchainExtent = {0,0};
			VkSurfaceKHR surface = VK_NULL_HANDLE;
		};
		struct swapChainSupportDetails
		{
			VkSurfaceCapabilitiesKHR capabilities;
			std::vector<VkSurfaceFormatKHR> formats;
			std::vector<VkPresentModeKHR> presentModes;
		};
		class vulkanSwapchain
		{
		public:
			vulkanSwapchain(const swapchainSpec& swapChainSpec);
			virtual ~vulkanSwapchain();
			VkResult createSwapchain(const swapchainSpec& swapChainSpec);
			VkResult recreateSwapchain();
		private:
			VkResult destroySwapchain();
			swapChainSupportDetails querySwapChainSupport(const VkPhysicalDevice& device);
			VkSurfaceFormatKHR chooseSwapSurfaceFormat(const std::vector<VkSurfaceFormatKHR>& availableFormats);
			VkPresentModeKHR chooseSwapPresentMode(const std::vector<VkPresentModeKHR>& availablePresentModes);
			VkExtent2D chooseSwapExtent(const VkSurfaceCapabilitiesKHR& capabilities);
			/*helper functions*/
			swapchainSpec mSwapchainSpec;
		};
	}
}


