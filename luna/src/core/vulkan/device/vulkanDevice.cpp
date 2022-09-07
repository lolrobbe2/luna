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
		}

		void vulkanDevice::destroyContext()
		{
		}




		/* private helper functions */
		VkResult vulkanDevice::createInstance()
		{
			deviceHandle.appInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
			deviceHandle.appInfo.pApplicationName = "app";
			deviceHandle.appInfo.applicationVersion = VK_MAKE_API_VERSION(0, 0, 0, 0);
			deviceHandle.appInfo.pEngineName = "luna";
			deviceHandle.appInfo.engineVersion = VK_MAKE_API_VERSION(0, 0, 1, 0);
			deviceHandle.appInfo.apiVersion = VK_API_VERSION_1_1;


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
	}
}
