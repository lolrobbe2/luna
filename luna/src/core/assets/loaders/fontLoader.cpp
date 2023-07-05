#include "fontLoader.h"
#include <core/vulkan/utils/vulkanAllocator.h>
#include <core/vulkan/rendering/vulkanTexture.h>
namespace luna
{
	namespace assets
	{

		/**
		 * @brief allocates and creates the atlas texture from wich glyphs can be sampled from.
		 *
		 */
		static void createFontTexture(int width, int height, VkImage* imageHandle, VkImageView* imageViewHandle, VkFormat imageFormat)
		{
			int imageSize = width * height; //fonts are always single channel;
			VkResult result = utils::vulkanAllocator::createImage(imageHandle, VK_IMAGE_USAGE_SAMPLED_BIT | VK_IMAGE_USAGE_TRANSFER_DST_BIT, VMA_MEMORY_USAGE_GPU_ONLY, { (unsigned int)width,(unsigned int)height,1 }, imageFormat);
			utils::vulkanAllocator::createImageView(imageViewHandle, *imageHandle, imageFormat, VK_IMAGE_ASPECT_COLOR_BIT);
		}

		ref<asset> fontLoader::loadFont(assetHandle handle, assetMetadata* metadata)
		{
			LN_PROFILE_FUNCTION();
			fontAssetMetadata* fontMetadata = (fontAssetMetadata*)metadata;
			
			VkBuffer imageBuffer = VK_NULL_HANDLE;
			VkImage imageHandle = VK_NULL_HANDLE;
			VkImageView imageViewHandle = VK_NULL_HANDLE;

			utils::vulkanAllocator::createBuffer(&imageBuffer, FONT_ATLAS_WIDTH * FONT_ATLAS_HEIGHT, VK_BUFFER_USAGE_STORAGE_TEXEL_BUFFER_BIT | VK_BUFFER_USAGE_TRANSFER_SRC_BIT, VMA_MEMORY_USAGE_CPU_TO_GPU, VMA_ALLOCATION_CREATE_MAPPED_BIT);
			void* data = utils::vulkanAllocator::getAllocationInfo((uint64_t)imageBuffer).pMappedData;

			VkFormat imageFormat = utils::vulkanAllocator::getSuitableFormat(VK_BUFFER_USAGE_STORAGE_TEXEL_BUFFER_BIT | VK_BUFFER_USAGE_TRANSFER_SRC_BIT, 0);
			createFontTexture(FONT_ATLAS_WIDTH, FONT_ATLAS_HEIGHT, &imageHandle, &imageViewHandle, imageFormat);

			memcpy_s(data, sizeof(fontAtlas), &fontMetadata->atlas, sizeof(fontAtlas));
			
			return ref<asset>(new vulkan::vulkanTexture((uint64_t)imageViewHandle, imageBuffer, imageHandle, imageViewHandle, { fontMetadata->width,fontMetadata->height }));
		}
	}
}