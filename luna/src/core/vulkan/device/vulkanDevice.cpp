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
			swapchain = std::shared_ptr<vulkan::vulkanSwapchain> (new vulkan::vulkanSwapchain(swapchainspec));
			
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
			swapchain->frameBuffers.resize(swapchain->mSwapchain.image_count);
			for (size_t i = 0; i < swapchain->mSwapchain.image_count; i++) {
				VkImageView attachments[] = {swapchain->mSwapchain.get_image_views().value()[i]};
				
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
				.set_engine_version(MAJOR, MINOR, PATCH)
				.use_default_debug_messenger()
				.set_debug_callback(debugCallback)
				.require_api_version(1, 2, 0);
			#ifdef LN_DEBUG
				instanceBuilder.request_validation_layers(true);
			#else
				instanceBuilder.request_validation_layers(false);
			#endif // LN_DEBUG

			for (const auto& extension : getRequiredExtensions())
			{
				instanceBuilder.enable_extension(extension);
			}
			auto instance = instanceBuilder.build();
			if (instance) 
			{
				deviceHandle.instance = instance.value();
				deviceHandle.appInfo.apiVersion = VKB_VK_API_VERSION_1_2;
				return VK_SUCCESS;
			}
			LN_CORE_ERROR("error creating instance: {0}", instance.error().message());
			return (VkResult)instance.error().value();
		}

		VkResult vulkanDevice::pickPhysicalDevice()
		{
			vkb::PhysicalDeviceSelector deviceSelector{ deviceHandle.instance };
			VkPhysicalDeviceFeatures features {};
			VkPhysicalDeviceVulkan12Features features12{ VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_VULKAN_1_2_FEATURES };
			
			
			
			
			deviceSelector
				.set_minimum_version(1, 2)
				.set_surface(surface);
			auto tempPhysicalDevice = deviceSelector.select().value();
			if (tempPhysicalDevice.physical_device != VK_NULL_HANDLE) 
			{

				vkGetPhysicalDeviceFeatures(tempPhysicalDevice, &supportedFeatures);
				if (supportedFeatures.shaderStorageImageArrayDynamicIndexing == VK_TRUE) features12.shaderSampledImageArrayNonUniformIndexing = VK_TRUE; //shaderStorageImageArrayDynamicIndexing : specifies whether arrays of storage images can be indexed by dynamically uniform integer expressions in shader code
				else  LN_CORE_ERROR("featurenot supported: shaderStorageImageArrayDynamicIndexing");
				if (supportedFeatures.multiViewport == VK_TRUE) features.multiViewport = VK_TRUE;
				auto physicalDevice = deviceSelector
					.set_required_features(features)
					.set_required_features_12(features12)
					.select();
				if (physicalDevice)
				{
					deviceHandle.physicalDevice = physicalDevice.value();
					LN_CORE_INFO("chosen gpu = {0}", deviceHandle.physicalDevice.name);
					return VK_SUCCESS;
				}
				LN_CORE_ERROR("error choosing gpu: {0}", physicalDevice.error().message());
				return (VkResult)physicalDevice.error().value();
			}
			return VK_ERROR_DEVICE_LOST;
		}

		VkResult vulkanDevice::createLogicalDevice()
		{
			vkb::DeviceBuilder deviceBuilder{ deviceHandle.physicalDevice };
			auto device = deviceBuilder.build();
			if (device)
			{
				deviceHandle.device = device.value();
				return VK_SUCCESS;
			}
			LN_CORE_ERROR("could not create logical device: {0}", device.error().message());
			return (VkResult)device.error().value();
		}


		/*private helper helper functions */
		

		std::vector<const char*> vulkanDevice::getRequiredExtensions() 
		{
			uint32_t glfwExtensionCount = 0;
			const char** glfwExtensions;
			glfwExtensions = glfwGetRequiredInstanceExtensions(&glfwExtensionCount);

			std::vector<const char*> extensions(glfwExtensions, glfwExtensions + glfwExtensionCount);
			
			return extensions;
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
				LN_CORE_ERROR((std::string)pCallbackData->pMessage);
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
