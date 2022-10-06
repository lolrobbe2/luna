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

		/**
		 * @brief the vulkanSwapchain is a collection of vulkan frambuffers.
		 */
		class vulkanSwapchain
		{
		public:
			vulkanSwapchain(const swapchainSpec& swapChainSpec);
			virtual ~vulkanSwapchain();
			/**
			 * @brief creates the swapchain.
			 * 
			 * \param swapChainSpec specification for the swapchain type.
			 * \return VK_SUCCESS whe the swapchain was created succesfuly
			 * \return for appropriate error info see: https://registry.khronos.org/vulkan/specs/1.3-extensions/man/html/vkCreateSwapchainKHR.html
			 */
			VkResult createSwapchain(const swapchainSpec& swapChainSpec);
			/**
			 * @brief recreates the swapchain.
			 * 
			 * \return returns VK_SUCCESS on succesful creation.
			 */
			VkResult recreateSwapchain();
			VkViewport getViewport();
			VkRect2D getScissor();
			inline VkFormat getSurfaceFormat() { return swapchainImageFormat; };
		private:
			/**
			 * @brief destroys the device bound swapchain.
			 * 
			 * \return VK_SUCCES
			 */
			VkResult destroySwapchain();
			/**
			 * @brief gets the swapchain support details.
			 * 
			 * \param const VkPhysicalDevice& handle to the physicalDevice(GPU)
			 * \return swapChainSupportDetails struct
			 */
			swapChainSupportDetails querySwapChainSupport(const VkPhysicalDevice& device);
			/**
			 * gets the swapchain supported surface format.
			 * 
			 * \param std::vector<VkSurfaceFormatKHR> of availableFormats
			 * \return VkSurfaceFormatKHR surfaceFormat
			 */
			VkSurfaceFormatKHR chooseSwapSurfaceFormat(const std::vector<VkSurfaceFormatKHR>& availableFormats);
			/**
			 * @brief chooseSwaPresentMode automaticly selects the present mode.
			 * 
			 * \param availablePresentModes
			 * \return VkpresentModeKHR
			 */
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


