#include <lnpch.h>
#include <core/vulkan/device/vulkanDevice.h>
#include <core/utils/shaderLibrary.h>

namespace luna
{
	namespace vulkan
	{
		vulkanDevice::vulkanDevice(const ref<vulkan::window>& Window)
		{
			window = Window;
			createContext();
		}
		vulkanDevice::~vulkanDevice()
		{
			destroyContext();
		}
		void vulkanDevice::createContext()
		{
			
			LN_CORE_TRACE("instance creation result = {0}",createInstance());
			LN_CORE_TRACE("surface creation result = {0}", glfwCreateWindowSurface(deviceHandle.instance, (GLFWwindow*)window->getWindow(), nullptr, &surface));
			LN_CORE_TRACE("device selection result = {0}", pickPhysicalDevice());
			LN_CORE_TRACE("logical device creation result = {0}", createLogicalDevice()); //TODO add specification;
			vulkan::swapchainSpec swapchainspec;
			swapchainspec.device = deviceHandle.device;
			swapchainspec.indices = queueFamily;
			swapchainspec.physicalDevice = deviceHandle.physicalDevice;
			swapchainspec.surface = surface;
			swapchainspec.swapchainExtent = { window->getWidth(),window->getHeight() };
			swapchainspec.window = window;
			swapchain = std::shared_ptr < vulkan::vulkanSwapchain > (new vulkan::vulkanSwapchain(swapchainspec));
			utils::shaderLibrary::init();
		}

		void vulkanDevice::destroyContext()
		{
			swapchain->~vulkanSwapchain();
			vkDestroySurfaceKHR(deviceHandle.instance, surface, nullptr);
			vkDestroyDevice(deviceHandle.device, nullptr);
			vkDestroyInstance(deviceHandle.instance, nullptr);
		}
		//TODO needs to be placed in swapchain.
		VkResult vulkanDevice::createFramebuffers(VkRenderPass renderPass)
		{
			VkResult result;
			swapchain->frameBuffers.resize(swapchain->mSwapchain.get_image_views().value().size());
			for (size_t i = 0; i < swapchain->mSwapchain.image_count; i++) {
				VkImageView attachments[] = {
					swapchain->mSwapchain.get_image_views().value()[i]
				};
				
				VkFramebufferCreateInfo framebufferInfo{};
				framebufferInfo.sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;
				framebufferInfo.renderPass = renderPass;
				framebufferInfo.attachmentCount = 1;
				framebufferInfo.pAttachments = attachments;
				framebufferInfo.width = swapchain->mSwapchain.extent.width;
				framebufferInfo.height = swapchain->mSwapchain.extent.height;
				framebufferInfo.layers = 1;
				
				result = vkCreateFramebuffer(deviceHandle.device, &framebufferInfo, nullptr, &swapchain->frameBuffers[i]);
			}
			return result;
		}

		vulkanDevice::deviceHandles vulkanDevice::getDeviceHandles()
		{
			return deviceHandle;
		}




		/* private helper functions */
		VkResult vulkanDevice::createInstance()
		{
			vkb::InstanceBuilder instanceBuilder;
			instanceBuilder.set_app_name("app")
				.set_engine_name("luna software engine")
				.request_validation_layers(true)
				.use_default_debug_messenger()
				.require_api_version(1, 1, 0)
				.set_debug_callback(debugCallback);
			for (const auto& extension : getRequiredExtensions())
			{
				instanceBuilder.enable_extension(extension);
			}
			deviceHandle.instance = instanceBuilder.build().value();
			return VK_SUCCESS;
		}

		VkResult vulkanDevice::pickPhysicalDevice()
		{
			vkb::PhysicalDeviceSelector deviceSelector{ deviceHandle.instance };
			deviceSelector
				.set_minimum_version(1, 1)
				.set_surface(surface);
			auto physicalDevice = deviceSelector.select();
			deviceHandle.physicalDevice = physicalDevice.value();
			LN_CORE_INFO("chosen gpu = {0}", deviceHandle.physicalDevice.name);
			return VK_SUCCESS;
		}

		VkResult vulkanDevice::createLogicalDevice()
		{
			vkb::DeviceBuilder deviceBuilder{ deviceHandle.physicalDevice };
			deviceHandle.device = deviceBuilder.build().value();
			return VK_SUCCESS;
		}


		/*private helper helper functions */
		VkResult vulkanDevice::checkValidationLayerSupport(const std::vector<const char*>& validationLayers)
		{
			uint32_t layerCount;
			bool spacer = 0;
			vkEnumerateInstanceLayerProperties(&layerCount, nullptr);
			std::vector<VkLayerProperties> availableLayers(layerCount);
			vkEnumerateInstanceLayerProperties(&layerCount, availableLayers.data());
			uint8_t layersFound = 0;
			for (const std::string layerName : validationLayers)
			{

				for (const auto& layerProperties : availableLayers)
				{
					if (strcmp(layerName.c_str(),layerProperties.layerName) == false)
					{
						layersFound++;
						break;
					}
					else if (strcmp(availableLayers.end()->layerName, layerProperties.layerName) == true)
					{
						LN_CORE_ERROR("could not find validation layer: {0}", layerName);
					}
				}
			}
			if (layersFound == validationLayers.size()) return VK_SUCCESS;
			else return VK_ERROR_LAYER_NOT_PRESENT;
		}

		std::vector<const char*> vulkanDevice::getRequiredExtensions() 
		{
			uint32_t glfwExtensionCount = 0;
			const char** glfwExtensions;
			glfwExtensions = glfwGetRequiredInstanceExtensions(&glfwExtensionCount);

			std::vector<const char*> extensions(glfwExtensions, glfwExtensions + glfwExtensionCount);
			#ifdef ENABLE_VALIDATION_LAYERS
			extensions.push_back(VK_EXT_DEBUG_UTILS_EXTENSION_NAME);
			#endif // ENABLE_VALIDATION_LAYERS
			return extensions;
		}

		int vulkanDevice::rateDeviceSuitability(VkPhysicalDevice device)
		{
			VkPhysicalDeviceProperties deviceProperties;
			VkPhysicalDeviceFeatures deviceFeatures;
			vkGetPhysicalDeviceProperties(device, &deviceProperties);
			vkGetPhysicalDeviceFeatures(device, &deviceFeatures);
			int score = 0;
			// Discrete GPUs have a significant performance advantage
			if (deviceProperties.deviceType == VK_PHYSICAL_DEVICE_TYPE_DISCRETE_GPU) score += 1000;
			// Maximum possible size of textures affects graphics quality
			score += deviceProperties.limits.maxImageDimension2D;
			// Application can't function without geometry shaders
			if (!deviceFeatures.geometryShader) return 0;
			return score;

		}
		VkDeviceQueueCreateInfo* vulkanDevice::createQueues()
		{
			vulkan::queueFamilyIndices indices;
			uint32_t queueFamilyCount = 0;
			int i = 0;
			vkGetPhysicalDeviceQueueFamilyProperties(deviceHandle.physicalDevice, &queueFamilyCount, nullptr);

			std::vector<VkQueueFamilyProperties> queueFamilies(queueFamilyCount);
			vkGetPhysicalDeviceQueueFamilyProperties(deviceHandle.physicalDevice, &queueFamilyCount, queueFamilies.data());

			VkBool32 presentSupport = false;
			vkGetPhysicalDeviceSurfaceSupportKHR(deviceHandle.physicalDevice, i, surface, &presentSupport);

			if (presentSupport) indices.presentFamily = i;
			for (const auto& queueFamily : queueFamilies) {
				if (queueFamily.queueFlags & VK_QUEUE_GRAPHICS_BIT) {indices.graphicsFamily = i;};
				if (queueFamily.queueFlags & VK_QUEUE_TRANSFER_BIT) {indices.transferFamily = i;};
				if (indices.isComplete()) {break; };
				i++;
			}
			queueFamily = indices;
			std::set<uint32_t> uniqueQueueFamilies = { indices.graphicsFamily.value(), indices.presentFamily.value(),indices.transferFamily.value() };

			
			for (uint32_t queueFamily : uniqueQueueFamilies) 
			{
				VkDeviceQueueCreateInfo queueCreateInfo{};
				queueCreateInfo.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
				queueCreateInfo.queueFamilyIndex = queueFamily;
				queueCreateInfo.queueCount = 1;
				queueCreateInfo.pQueuePriorities = queuePriorities.data();
				queueCreateInfos.push_back(queueCreateInfo);
			}
			return queueCreateInfos.data();
		}
		VKAPI_ATTR VkBool32 VKAPI_CALL vulkanDevice::debugCallback(
			VkDebugUtilsMessageSeverityFlagBitsEXT           messageSeverity,
			VkDebugUtilsMessageTypeFlagsEXT                  messageTypes,
			const VkDebugUtilsMessengerCallbackDataEXT* pCallbackData,
			void* pUserData)
		{
			switch (messageSeverity)
			{
			case VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT:
				LN_CORE_TRACE(pCallbackData->pMessage);
				break;
			case VK_DEBUG_UTILS_MESSAGE_SEVERITY_INFO_BIT_EXT:
				LN_CORE_INFO(pCallbackData->pMessage);
				break;
			case VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT:
				LN_CORE_WARN(pCallbackData->pMessage);
				break;
			case VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT:
				LN_CORE_ERROR(pCallbackData->pMessage);
				break;
			case VK_DEBUG_UTILS_MESSAGE_SEVERITY_FLAG_BITS_MAX_ENUM_EXT:
				break;
			default:
				LN_CORE_CRITICAL(pCallbackData->pMessage);
				break;
			}
			return VK_SUCCESS;
		};
	}
}
