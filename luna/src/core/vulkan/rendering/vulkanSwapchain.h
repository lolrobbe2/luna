#pragma once
#include <core/vulkan/window/window.h>
namespace luna
{
	namespace vulkan
	{
		struct queueFamilyIndices
		{
			std::optional<uint32_t> graphicsFamily;
			std::optional<uint32_t> transferFamily;
			std::optional<uint32_t> presentFamily;

			bool isComplete() {
				return graphicsFamily.has_value() && presentFamily.has_value() && transferFamily.has_value();
			}
		};
		struct swapchainSpec
		{
			VkPhysicalDevice physicalDevice;
			VkDevice device;
			std::shared_ptr<vulkan::window> window = nullptr;
			VkExtent2D swapchainExtent = {0,0};
			VkSurfaceKHR surface = VK_NULL_HANDLE;
			queueFamilyIndices indices;
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
			void createImageViews();
			VkImageView createImageView(VkImage image, VkFormat format);
			/*helper functions*/
			swapchainSpec mSwapchainSpec;
			VkSwapchainKHR mSwapchain;
			std::vector<VkImage> swapchainImages;
			std::vector<VkImageView> swapChainImageViews;
			VkFormat swapchainImageFormat;

		};
	}
}


