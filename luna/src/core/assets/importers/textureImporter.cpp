#include "textureImporter.h"
#include <core/assets/assetImporter.h>
#include <core/artemis/device/allocator.h>

#include <core/assets/publicTypes/image.h>
namespace luna
{
	namespace assets 
	{

		ref<asset> textureImporter::importTexture(assetHandle handle, assetMetadata* metadata)
		{
			LN_PROFILE_FUNCTION();
			textureAssetMetadata* textureMetaData = (textureAssetMetadata*)metadata;
			std::string filePath = reinterpret_cast<char*>(textureMetaData->baseMetaData.filePath);
			filePath += "\\";
			filePath += reinterpret_cast<char*>(textureMetaData->baseMetaData.name);
			metadata->fileSizeBytes = std::filesystem::file_size(filePath);

			ref<artemis::allocator> p_allocator = assetImporter::getAllocator();

			if (!std::filesystem::exists(filePath)) { LN_CORE_ERROR("file does not exist: {}", filePath); return ref<asset>(); }

			int width, height, channels;
			stbi_uc* stbImage = stbi_load(filePath.c_str(), &width, &height, &channels, 4);

			if (channels == 3) channels = 4; //RGB formats are most likely not supported! so convert to quad channels.
			
			uint64_t imageSize = width * height * channels;

			artemis::buffer& buffer = p_allocator->allocateBuffer(imageSize, artemis::CPU_TO_GPU, VK_BUFFER_USAGE_STORAGE_TEXEL_BUFFER_BIT | VK_BUFFER_USAGE_TRANSFER_SRC_BIT);

			buffer.setData(stbImage, imageSize);
			stbi_image_free(stbImage);

			artemis::image& image = p_allocator->allocateImage({ width,height }, channels, VK_IMAGE_USAGE_SAMPLED_BIT | VK_IMAGE_USAGE_TRANSFER_DST_BIT, artemis::GPU_ONLY);
			p_allocator->transitionImageLayoutFront(image, VK_IMAGE_LAYOUT_UNDEFINED, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL);
			p_allocator->copyBufferToImage(buffer, image);
			p_allocator->transitionImageLayoutBack(image, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, VK_IMAGE_LAYOUT_GENERAL);
			textureMetaData->channels = channels;
			textureMetaData->width = (uint32_t)width;
			textureMetaData->height = (uint32_t)height;

			return std::dynamic_pointer_cast<asset>(createRef<assets::image>(image));
			//return ref<asset>(new vulkan::vulkanTexture(_handle, buffer, imageHandle, imageViewHandle, glm::vec2({ width,height })));
		}
	}
}