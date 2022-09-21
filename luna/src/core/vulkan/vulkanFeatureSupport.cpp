#include "vulkanFeatureSupport.h"
namespace luna
{
    namespace vulkan
    {
        VkResult vulkanFeatureSupport::init(const VkPhysicalDevice& physicalDevice) 
        {
            sPhysicalDevice = physicalDevice;
            return VkResult();
        }
        std::pair<VkResult, VkFormatFeatureFlags> vulkanFeatureSupport::getFormatFeatures(const VkFormat& format, VkFormatFeatureFlags requestedFeatures)
        {
            VkFormatProperties formatProperties{};
            vkGetPhysicalDeviceFormatProperties(sPhysicalDevice, format, &formatProperties);
            VkFormatFeatureFlags availableFeatureFlags = formatProperties.bufferFeatures | formatProperties.linearTilingFeatures | formatProperties.optimalTilingFeatures;
            VkFormatFeatureFlags finalFeatureFlags = availableFeatureFlags& requestedFeatures;
            VkResult result = VK_SUCCESS;
            if (finalFeatureFlags != requestedFeatures) { result = VK_ERROR_FORMAT_NOT_SUPPORTED; };
            return { result, finalFeatureFlags};
        }
    }
}
