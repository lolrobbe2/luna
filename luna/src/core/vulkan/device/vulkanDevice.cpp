#include <lnpch.h>
#include <core/vulkan/device/vulkanDevice.h>
namespace luna
{
	namespace renderer
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
		}

		void vulkanDevice::destroyContext()
		{
			swapchain->~vulkanSwapchain();
			vkDestroySurfaceKHR(deviceHandle.instance, surface, nullptr);
			vkDestroyInstance(deviceHandle.instance, nullptr);
			vkDestroyDevice(deviceHandle.device, nullptr);
		}




		/* private helper functions */
		VkResult vulkanDevice::createInstance()
		{
			deviceHandle.appInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
			deviceHandle.appInfo.pApplicationName = "app";
			deviceHandle.appInfo.applicationVersion = VK_MAKE_API_VERSION(0, 0, 0, 0);
			deviceHandle.appInfo.pEngineName = "luna";
			deviceHandle.appInfo.engineVersion = VK_MAKE_API_VERSION(0, 0, 1, 0);
			deviceHandle.appInfo.apiVersion = VK_API_VERSION_1_2;


			VkInstanceCreateInfo createInfo{};
			createInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
			createInfo.pApplicationInfo = &deviceHandle.appInfo;
			createInfo.enabledLayerCount = 0;

			std::vector<const char*> requiredLayers
			{
				"VK_LAYER_KHRONOS_validation"
			};
			if (checkValidationLayerSupport(requiredLayers) != VK_SUCCESS) { LN_CORE_ERROR("could not find all required layers!"); return VK_ERROR_LAYER_NOT_PRESENT; };
			
			createInfo.ppEnabledLayerNames = requiredLayers.data();
			createInfo.enabledLayerCount = (uint32_t)requiredLayers.size();

			std::vector<const char*> requiredExtensions = getRequiredExtensions();
			
			createInfo.ppEnabledExtensionNames = requiredExtensions.data();
			createInfo.enabledExtensionCount = requiredExtensions.size();
			return vkCreateInstance(&createInfo, nullptr, &deviceHandle.instance);
		}

		VkResult vulkanDevice::pickPhysicalDevice()
		{
			uint32_t deviceCount = 0;
			vkEnumeratePhysicalDevices(deviceHandle.instance, &deviceCount, nullptr);
			std::vector<VkPhysicalDevice> devices(deviceCount);
			vkEnumeratePhysicalDevices(deviceHandle.instance, &deviceCount, devices.data());
			std::multimap<int, VkPhysicalDevice> PDcandidates;
			for (const auto& device : devices) {
				int score = rateDeviceSuitability(device);
				PDcandidates.insert(std::make_pair(score, device));
			}
			if (PDcandidates.rbegin()->first == 0) return VK_ERROR_DEVICE_LOST;
			deviceHandle.physicalDevice = PDcandidates.rbegin()->second;
			VkPhysicalDeviceProperties deviceProperties;
			vkGetPhysicalDeviceProperties(deviceHandle.physicalDevice, &deviceProperties);
			uint32_t variant, major, minor, patch;
			GET_UNCODED_VERSION(deviceProperties.apiVersion, variant, major, minor, patch);
			LN_CORE_INFO("api version = {0}.{1}.{2}.{3}", variant, major, minor, patch);
			//chacking if api version is compatible.
			if (variant <= LN_VULKAN_VARIANT && major <= LN_VULKAN_MAJOR)
			{
				if (minor < LN_VULKAN_MINOR) {LN_CORE_INFO("picked physical device, name: {0}, type {1}", deviceProperties.deviceName, deviceProperties.deviceType); return VK_SUCCESS;};
				if (minor > LN_VULKAN_MINOR) return VK_ERROR_INCOMPATIBLE_DRIVER;
				if (patch > LN_VULKAN_PATCH) return VK_ERROR_INCOMPATIBLE_DRIVER;
			}
			LN_CORE_INFO("picked physical device, name: {0}, type {1}", deviceProperties.deviceName, deviceProperties.deviceType);
			return VK_SUCCESS;
		}

		VkResult vulkanDevice::createLogicalDevice()
		{
			createQueues();
			VkPhysicalDeviceFeatures deviceFeatures{};
			deviceFeatures.samplerAnisotropy = VK_TRUE;
			deviceFeatures.sparseBinding = VK_TRUE;

			VkDeviceCreateInfo createInfo{};
			createInfo.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;

			createInfo.queueCreateInfoCount = static_cast<uint32_t>(queueCreateInfos.size());
			createInfo.pQueueCreateInfos = queueCreateInfos.data();

			createInfo.pEnabledFeatures = &deviceFeatures;

			const std::vector<const char*> deviceExtensions = {VK_KHR_SWAPCHAIN_EXTENSION_NAME};
			createInfo.enabledExtensionCount = static_cast<uint32_t>(deviceExtensions.size());
			createInfo.ppEnabledExtensionNames = deviceExtensions.data();
			#ifdef ENABLE_VALIDATION_LAYERS
			std::vector<const char*> requiredLayers{"VK_LAYER_KHRONOS_validation"};
			createInfo.enabledLayerCount = static_cast<uint32_t>(requiredLayers.size());
			createInfo.ppEnabledLayerNames = requiredLayers.data();
			#else
				createInfo.enabledLayerCount = 0;
			#endif


			VkPhysicalDeviceShaderDrawParametersFeatures shaderDrawParametersFeatures = {};
			shaderDrawParametersFeatures.sType = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_SHADER_DRAW_PARAMETERS_FEATURES;
			shaderDrawParametersFeatures.pNext = nullptr;
			shaderDrawParametersFeatures.shaderDrawParameters = VK_TRUE;

			createInfo.pNext = &shaderDrawParametersFeatures;

			return vkCreateDevice(deviceHandle.physicalDevice, &createInfo, nullptr, &deviceHandle.device);
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
	}
}
