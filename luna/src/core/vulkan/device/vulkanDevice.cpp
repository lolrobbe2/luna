#include <lnpch.h>
#include <core/vulkan/device/vulkanDevice.h>
namespace luna
{
	namespace renderer
	{
		vulkanDevice::vulkanDevice()
		{
			createContext();
		}
		vulkanDevice::~vulkanDevice()
		{
			destroyContext();
		}
		void vulkanDevice::createContext()
		{
			LN_CORE_TRACE("instance creation result = {0}",createInstance());
			LN_CORE_TRACE("device selection result = {0}", pickPhysicalDevice());
		}

		void vulkanDevice::destroyContext()
		{
			vkDestroyInstance(deviceHandle.instance,nullptr);
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
			
			if (variant <= LN_VULKAN_VARIANT && major <= LN_VULKAN_MAJOR)
			{
				if (minor < LN_VULKAN_MINOR) {LN_CORE_INFO("picked physical device, name: {0}, type {1}", deviceProperties.deviceName, deviceProperties.deviceType); return VK_SUCCESS;};
				if (minor > LN_VULKAN_MINOR) return VK_ERROR_INCOMPATIBLE_DRIVER;
				if (patch > LN_VULKAN_PATCH) return VK_ERROR_INCOMPATIBLE_DRIVER;
			}
			LN_CORE_INFO("picked physical device, name: {0}, type {1}", deviceProperties.deviceName, deviceProperties.deviceType);
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
				}
			}
			if (layersFound == validationLayers.size()) return VK_SUCCESS;
			else return VK_ERROR_LAYER_NOT_PRESENT;
		}

		const std::vector<const char*>& vulkanDevice::getRequiredExtensions() 
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
	}
}
