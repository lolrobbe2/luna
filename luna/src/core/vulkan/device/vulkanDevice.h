#pragma once
#include <core/rendering/device.h>
#include <core/vulkan/rendering/vulkanSwapchain.h>
#include <core/rendering/shader.h>
#define GET_UNCODED_VERSION(version,variant, major, minor, patch) (uint32_t)variant = ((uint32_t)(version) >> 29); (uint32_t)major = (((uint32_t)(version) >> 22) & 0x7FU); (uint32_t)minor = (((uint32_t)(version) >> 12) & 0x3FFU) ;(uint32_t)patch = ((uint32_t)(version) & 0x3FFU);
#define LN_VULKAN_VARIANT 0
#define LN_VULKAN_MAJOR 1
#define LN_VULKAN_MINOR 3
#define LN_VULKAN_PATCH 216
namespace luna
{
	namespace vulkan
	{
		/**
		 * this is the vulkan device class and creates a rendering context.
		 * @code
		 * ref<device> rederingDevice = std::make_shared<device>(new vulkanDevice(window));
		 * @endcode
		 */
		class vulkanDevice : public renderer::device
		{
		public:
			/**
			 * @creates a vulkan device using the ref to the main window.
			 * 
			 * \param Window
			 */
			vulkanDevice(const ref<vulkan::window>& Window);
			virtual ~vulkanDevice();
			/**
			 * @brief creates the device context.
			 * 
			 */
			void createContext() override;
			/**
			 * detroys the device context.
			 * 
			 */
			void destroyContext() override;
			/**
			 * @brief returns the swapchain viewport.
			 */
			inline VkViewport getViewport() { return swapchain->getViewport(); };
			/**
			 * @brief returns the swapchain scissor.
			 */
			inline VkRect2D getScissor() { return swapchain->getScissor(); };
			/**
			 * @brief returns the swapchain format.
			 */
			inline VkFormat getSwapFormat() { return swapchain->mSwapchain.image_format; };
			/**
			 * @brief returns the queue index.
			 * \param vkb:QueueType type
			 */
			inline uint32_t getQueueIndex(const vkb::QueueType& type) { return deviceHandle.device.get_queue_index(type).value(); };
			/**
			 * @brief returns a queue based on the requested type .
			 * \param vkb::QueueType type 
			 */
			inline VkQueue getQueue(const vkb::QueueType& type) { return deviceHandle.device.get_queue(type).value(); };
			std::shared_ptr<vulkan::vulkanSwapchain> swapchain;
			/**
			 * @brief creates the appropriate framebuffers for the swapchain.
			 * 
			 * \param renderPass
			 * \return VK_SUCCESS when buffer creation whas succesful
			 */
			VkResult createFramebuffers(VkRenderPass renderPass);
		private:
			/**
			 * @name primaryHelperFunctions helper functions used to create the device
			 */
			///@{
			/**
			 * @brief creates the vulkan instance.
			 * 
			 * \return VK_SUCCESS when creation whas succesful.
			 * \return VK_ERROR_LAYER_NOT_PRESENT when a requested validation layer is not supported 
			 */
			VkResult createInstance();
			/**
			 * @brief picks the most powerful rendering device.
			 * 
			 * \return VK_SUCCES when suitable physical device(GPU) was found.
			 * \return VK_ERROR_DEVICE_LOST when no suitable physical device(GPU) could be found.
			 * \return VK_ERROR_INCOMPATIBLE_DRIVER when the driver version is outdated or incompatible.
			 */
			VkResult pickPhysicalDevice();
			/**
			 * @brief creates the logical device.
			 * 
			 * \return VK_SUCCES
			 * \return for VK_ERROR look at the vulkan documentation: https://registry.khronos.org/vulkan/specs/1.3-extensions/man/html/vkCreateDevice.html
			 */
			VkResult createLogicalDevice();
			///@}
			/**
			 * @name secondaryHelperFunctions helper functions used by the primary helper functions
			 */
			///@{
			/**
			 * @brief checks if all requested validation layers are supported.
			 * 
			 * \param validationLayers requested validation layers.
			 * \return VK_SUCCES when all requested validation layers are supported.
			 * \return VK_ERROR_LAYER_NOT_PRESENT when a requested validation layer is not supported. 
			 */
			std::vector<const char*> getRequiredExtensions();
			/**
			 * @brief rates the gpu based on expected performance with a score.
			 * 
			 * \param device the physicalDevice(GPU) to be rated
			 * \return int the score the physical device has.
			 */
			VkDeviceQueueCreateInfo* createQueues();
			/**
			 * @brief the debug callback function for vulkan debug and error catching.
			 * 
			 * \param messageSeverity
			 * \param messageTypes
			 * \param pCallbackData
			 * \param pUserData
			 * \return 
			 */
			static VKAPI_ATTR VkBool32 VKAPI_CALL vulkanDevice::debugCallback(VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity,
				VkDebugUtilsMessageTypeFlagsEXT messageTypes,
				const VkDebugUtilsMessengerCallbackDataEXT* pCallbackData,
				void* pUserData);
			///@}
		public:
			struct deviceHandles
			{
				VkApplicationInfo appInfo{};
				vkb::Instance instance;
				vkb::Device device;
				vkb::PhysicalDevice physicalDevice;
			};
			deviceHandles getDeviceHandles();
		private:
			
			std::vector<float> queuePriorities = { 0.99f };
			VkSurfaceKHR surface = VK_NULL_HANDLE;
			deviceHandles deviceHandle;
			vulkan::queueFamilyIndices queueFamily;
			std::vector<VkDeviceQueueCreateInfo> queueCreateInfos;
		};
	}
}


