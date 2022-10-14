#include <core/core.h>
#include <core/rendering/device.h>
#include "lnpch.h"
namespace luna
{
	namespace utils
	{
		/**
		 * @brief static class that creates vulkan objects like buffers and images.
		 */
		class vulkanObjectFactory
		{
		public:
			/**
			 * @brief initializes the vulkan object factory.
			 */
			static VkResult init(ref<renderer::device> device);
			/**
			 * @brief creates a vulkan image.
			 * \param VkImage* pImage pointer to the image handle
			 * \param VkImageCreateInfo* pCreateInfo pointer to imageCreateInfo struct.
			 */
			static VkResult createImage(VkImage* pImage, VkImageCreateInfo* pCreateInfo);
			/**
			 * @brief creates a vulkan image.
			 * \param VkImage* pImage, pointer to the image handle
			 * \param VkFormat format, image format
			 * \param VkImageUsageFlags, usageFlags
			 * \param VkExtent3D extent, image extent (width height)
			 */
			static VkResult createImage(VkImage* pImage, VkImageUsageFlags usageFlags, VkExtent3D extent, VkFormat format = VK_FORMAT_B8G8R8A8_UNORM);
		private:
			inline static ref<renderer::device> pDevice; //ref to graphics device.
		};
	}
}
