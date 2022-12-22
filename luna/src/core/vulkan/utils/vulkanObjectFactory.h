#include <core/core.h>
#include <core/rendering/device.h>
#include "lnpch.h"
namespace luna
{
	namespace utils
	{
		/**
		 * @brief static class that creates vulkan objects like buffers and images.
		 * @note this class is only meant to be included in the .cpp files not in the .h files
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
			 * @see https://registry.khronos.org/vulkan/specs/1.3-extensions/man/html/VkImage.html
			 * @see https://registry.khronos.org/vulkan/specs/1.3-extensions/man/html/vkCreateImage.html
			 * @see https://registry.khronos.org/vulkan/specs/1.3-extensions/man/html/VkImageCreateInfo.html
			 */
			static VkResult createImage(VkImage* pImage, VkImageCreateInfo* pCreateInfo);
			/**
			 * @brief creates a vulkan image.
			 * \param VkImage* pImage, pointer to the image handle
			 * \param VkFormat format, image format
			 * \param VkImageUsageFlags, usageFlags
			 * \param VkExtent3D extent, image extent (width height)
			 * @see https://registry.khronos.org/vulkan/specs/1.3-extensions/man/html/VkImage.html
			 * @see https://registry.khronos.org/vulkan/specs/1.3-extensions/man/html/vkCreateImage.html
			 * @see https://registry.khronos.org/vulkan/specs/1.3-extensions/man/html/VkImageCreateInfo.html
			 */
			static VkResult createImage(VkImage* pImage, VkImageUsageFlags usageFlags, VkExtent3D extent, VkFormat format = VK_FORMAT_R8G8B8A8_UNORM);
			/**
			 * @brief creates a VkImageView.
			 * 
			 * \param pImageView* pointer to the imageView.
			 * \param pImageViewCreateInfo* pointer to the creareInfoStruct
			 * \return VK_SUCCES when creation was succesful.
			 * @see https://registry.khronos.org/vulkan/specs/1.3-extensions/man/html/VkImageView.html
			 * @see https://registry.khronos.org/vulkan/specs/1.3-extensions/man/html/vkCreateImageView.html
			 * @see https://registry.khronos.org/vulkan/specs/1.3-extensions/man/html/VkImageViewCreateInfo.html
			 */
			static VkResult createImageView(VkImageView* pImageView, VkImageViewCreateInfo* pImageViewCreateInfo);
			/**
			 * @brief creates a VkImageView.
			 * 
			 * \param pImageView* pointer to the imageView.
			 * \param VkImage image handle
			 * \param VkFormat format image format
			 * \param imageAspectFlags
			 * \return VK_SUCCES when creation was succesful.
			 * @see https://registry.khronos.org/vulkan/specs/1.3-extensions/man/html/VkImageView.html
			 * @see https://registry.khronos.org/vulkan/specs/1.3-extensions/man/html/vkCreateImageView.html
			 * @see https://registry.khronos.org/vulkan/specs/1.3-extensions/man/html/VkImageViewCreateInfo.html
			 */
			static VkResult createImageView(VkImageView* pImageView, const VkImage& image, const VkFormat& format, const VkImageAspectFlags& imageAspectFlags);
		private:
			inline static ref<renderer::device> pDevice; //ref to graphics device.
		};
	}
}
