#pragma once
#include <core/vulkan/window/window.h>
#include <VkBootstrap.h>

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
			VkDevice device;
			std::shared_ptr<vulkan::window> window = nullptr;
			VkExtent2D swapchainExtent = {0,0};
			VkSurfaceKHR surface = VK_NULL_HANDLE;
			queueFamilyIndices indices;
		};

		/**
		 * @brief the vulkanSwapchain is a collection of vulkan frambuffers and image views to wich the scene will be .
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
			/**
			 * @brief returns the main viewport handle.
			 * 
			 * \return VkViewport main handle.
			 */
			VkViewport getViewport();
			/**
			 * @brief returns the viewport scissor.
			 * 
			 * \return VkRect2D extent.
			 */
			VkRect2D getScissor();
			/**
			 * @brief initializes the main viewport.
			 * 
			 * \return VK_SUCCESS when the initialization was succesful.
			 */
			VkResult initViewport(uint32_t maxFramesInFlight);
			/**
			 * @brief recreates the swapchain.
			 * 
			 * \param maxFramesInFlight
			 * \return VK_SUCCES when recreation was succesful.
			 */
			VkResult recreateViewport(uint32_t maxFramesInFlight);
			/**
			 * @brief returns the surface format.
			 * 
			 * \param VkFormat
			 */
			inline VkFormat getSurfaceFormat() { return mSwapchain.image_format; };
			/**
			 * @brief returns the framebuffer given an index.
			 * 
			 * \param VkFrameBuffer the framebuffer handle.
			 */
			inline VkFramebuffer getFrameBuffer(uint8_t index) { return frameBuffers[index]; };
			/**
			 * @brief returns a ViewportImage descriptor.
			 * 
			 * \param VkDescriptorSet image descriptor.
			 */
			inline VkDescriptorSet getViewportImage(uint8_t currentFrame) { return m_Dset[currentFrame]; };
			std::vector<VkImage> sceneViewportImages;
			vkb::Swapchain mSwapchain;
			std::vector<VkFramebuffer> frameBuffers;
			std::vector<VkDescriptorSet> m_Dset;
		private:
			/**
			 * @brief destroys the device bound swapchain.
			 * 
			 * \return VK_SUCCES
			 */
			VkResult destroySwapchain();
			/*helper functions*/
			swapchainSpec mSwapchainSpec;
			
			std::vector<VkImage> swapchainImages;
			std::vector<VkImageView> swapChainImageViews;
			VkFormat swapchainImageFormat;
			bool init = false;
			VkSampler viewportSampler;
			VkSurfaceCapabilitiesKHR surfaceCapaBilities;
			std::vector<VkImageView> sceneViewportImageViews;
		};
	}
}


