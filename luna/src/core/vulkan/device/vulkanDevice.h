#pragma once
#include <core/rendering/device.h>
#include <vulkan/vulkan.h>
#define GET_UNCODED_VERSION(version,variant, major, minor, patch) (uint32_t)variant = ((uint32_t)(version) >> 29); (uint32_t)major = (((uint32_t)(version) >> 22) & 0x7FU); (uint32_t)minor = (((uint32_t)(version) >> 12) & 0x3FFU) ;(uint32_t)patch = ((uint32_t)(version) & 0x3FFU);
#define LN_VULKAN_VARIANT 0
#define LN_VULKAN_MAJOR 1
#define LN_VULKAN_MINOR 3
#define LN_VULKAN_PATCH 216
namespace luna
{
	namespace renderer
	{
		class vulkanDevice : public device
		{
		public:
			vulkanDevice();
			virtual ~vulkanDevice();
			void createContext() override;
			void destroyContext() override;
		private:
			VkResult createInstance();
			VkResult pickPhysicalDevice();
			/*helper helper functions*/
			VkResult checkValidationLayerSupport(const std::vector<const char*>& validationLayers);
			const std::vector<const char*>& getRequiredExtensions();
			int rateDeviceSuitability(VkPhysicalDevice device);

		private:
			
			struct deviceHandles
			{
				VkApplicationInfo appInfo{};
				VkInstance instance;
				VkDevice device;
				VkPhysicalDevice physicalDevice;
			};

			deviceHandles deviceHandle;
		};
	}
}


