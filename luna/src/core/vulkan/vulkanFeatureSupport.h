#pragma once
#include <core/vulkan/window/window.h>
namespace luna
{
	namespace vulkan
	{
		class vulkanFeatureSupport
		{
		public:
			static VkResult init(const VkPhysicalDevice& physicalDevice);
			static std::pair<VkResult, VkFormatFeatureFlags> getFormatFeatures(const VkFormat& format,VkFormatFeatureFlags requestedFeatures);
		private:
			inline static VkPhysicalDevice sPhysicalDevice;
		};
	}
}


