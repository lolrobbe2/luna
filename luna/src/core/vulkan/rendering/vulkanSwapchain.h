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
			vkb::PhysicalDevice physicalDevice;
			vkb::Device device;
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
			VkResult getNextImage(VkSemaphore presentSemaphore,uint32_t* imageIndex);
			VkViewport getViewport();
			VkRect2D getScissor();
			void createFramebuffers(VkRenderPass renderPass);
			inline VkFormat getSurfaceFormat() { return mSwapchain.image_format; };
			inline VkSwapchainKHR getSwapchain() { return mSwapchain; };
			inline VkFramebuffer getFrameBuffer(uint32_t index) { return swapchainFrameBuffers[index]; };
			vkb::Swapchain mSwapchain;
		private:
			/**
			 * @brief destroys the device bound swapchain.
			 * 
			 * \return VK_SUCCES
			 */
			VkResult destroySwapchain();
			/**
			 * @brief chooseSwaPresentMode automaticly selects the present mode.
			 * 
			 * \param availablePresentModes
			 * \return VkpresentModeKHR
			 */
			VkPresentModeKHR chooseSwapPresentMode(const std::vector<VkPresentModeKHR>& availablePresentModes);
			/*helper functions*/
			swapchainSpec mSwapchainSpec;
			
			std::vector<VkImage> swapchainImages;
			std::vector<VkImageView> swapChainImageViews;
			std::vector<VkFramebuffer> swapchainFrameBuffers;
			VkFormat swapchainImageFormat;

		};
	}
}


