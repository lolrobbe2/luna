#pragma once
#include <core/rendering/device.h>
#include <core/vulkan/rendering/vulkanSwapchain.h>
#define GET_UNCODED_VERSION(version,variant, major, minor, patch) (uint32_t)variant = ((uint32_t)(version) >> 29); (uint32_t)major = (((uint32_t)(version) >> 22) & 0x7FU); (uint32_t)minor = (((uint32_t)(version) >> 12) & 0x3FFU) ;(uint32_t)patch = ((uint32_t)(version) & 0x3FFU);
#define LN_VULKAN_VARIANT 0
#define LN_VULKAN_MAJOR 1
#define LN_VULKAN_MINOR 3
#define LN_VULKAN_PATCH 216
namespace luna
{
	namespace renderer
	{
		/**
		 * this is the vulkan device class and creates a rendering context.
		 * @code
		 * ref<device> rederingDevice = std::make_shared<device>(new vulkanDevice(window));
		 * @endcode
		 */
		class vulkanDevice : public device
		{
		public:
			vulkanDevice(const ref<vulkan::window>& Window);
			virtual ~vulkanDevice();
			void createContext() override;
			void destroyContext() override;

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
			VkResult checkValidationLayerSupport(const std::vector<const char*>& validationLayers);
			/**
			 * @brief returns all required vulkan extensions.
			 * 
			 * \return std::vector<const char*> required extensions for the engine
			 */
			std::vector<const char*> getRequiredExtensions();
			/**
			 * @brief rates the gpu based on expected performance with a score.
			 * 
			 * \param device the physicalDevice(GPU) to be rated
			 * \return int the score the physical device has.
			 */
			int rateDeviceSuitability(VkPhysicalDevice device);
			/**
			 * @brief creates all the command queues.
			 * 
			 * \return a pointer to all the QueueCreateInfo array
			 */
			VkDeviceQueueCreateInfo* createQueues();
			///@}
			std::shared_ptr<vulkan::vulkanSwapchain> swapchain;
		public:
		
		private:
			struct deviceHandles
			{
				VkApplicationInfo appInfo{};
				VkInstance instance;
				VkDevice device;
				VkPhysicalDevice physicalDevice;
			};
			std::vector<float> queuePriorities = { 0.99f };
			VkSurfaceKHR surface = VK_NULL_HANDLE;
			deviceHandles deviceHandle;
			vulkan::queueFamilyIndices queueFamily;
			std::vector<VkDeviceQueueCreateInfo> queueCreateInfos;
		};
	}
}


