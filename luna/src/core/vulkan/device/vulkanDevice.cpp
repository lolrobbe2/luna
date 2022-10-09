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
			
			
			LN_CORE_TRACE("crate instance result = {0}", createInstance());
			LN_CORE_TRACE("device selection result = {0}", pickPhysicalDevice());
			LN_CORE_TRACE("logical device creation result = {0}", createLogicalDevice()); //TODO add specification;
			vulkan::swapchainSpec swapchainspec;
			swapchainspec.device = deviceHandle.device;
			swapchainspec.indices = queueFamily;
			swapchainspec.physicalDevice = deviceHandle.physicalDevice;
			swapchainspec.surface = surface;
			swapchainspec.swapchainExtent = { window->getWidth(),window->getHeight() };
			swapchainspec.window = window;
			swapchain = std::shared_ptr <vulkan::vulkanSwapchain> (new vulkan::vulkanSwapchain(swapchainspec));
			utils::shaderLibrary::init();
		}

		void vulkanDevice::destroyContext()
		{
			swapchain->~vulkanSwapchain();
			vkDestroySurfaceKHR(deviceHandle.instance, surface, nullptr);
			vkDestroyDevice(deviceHandle.device, nullptr);
			vkDestroyInstance(deviceHandle.instance, nullptr);
		}

		vulkanDevice::deviceHandles vulkanDevice::getDeviceHandles()
		{
			return deviceHandle;
		}




		/* private helper functions */
		VkResult vulkanDevice::createInstance()
		{
			vkb::InstanceBuilder builder;
			builder.set_app_name("app")
				.require_api_version(1, 2, 0)
				.set_engine_name("luna engine")
				.enable_validation_layers(true)
				.use_default_debug_messenger();
			for (const auto& extension : getRequiredExtensions())
			{
				builder.enable_extension(extension);
			}
			auto result = builder.build();
			deviceHandle.instance = result.value();
			LN_CORE_CRITICAL("break point");
			return VK_SUCCESS;
		}

		VkResult vulkanDevice::pickPhysicalDevice()
		{
			vkb::PhysicalDeviceSelector selector{ deviceHandle.instance };
			glfwCreateWindowSurface(deviceHandle.instance, (GLFWwindow*)this->window->getWindow(), nullptr, &surface);
			deviceHandle.physicalDevice = selector
				.set_minimum_version(1, 1)
				.set_surface(surface)
				.select()
				.value();
			
			return VK_SUCCESS;
		}

		VkResult vulkanDevice::createLogicalDevice()
		{
			vkb::DeviceBuilder deviceBuilder{ deviceHandle.physicalDevice};
			vkb::detail::Result buildResult = deviceBuilder.build();
			deviceHandle.device = buildResult.value();
			return VK_SUCCESS;
		}


		/*private helper helper functions */

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
	}
}
