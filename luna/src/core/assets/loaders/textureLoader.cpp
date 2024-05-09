#include "textureLoader.h"
#include <core/artemis/rendering/image.h>
#include <core/artemis/device/buffer.h>
#include <core/assets/publicTypes/image.h>
#include <core/assets/assetLoader.h>
#include <core/artemis/device/allocator.h>
#include <core/debug/debugMacros.h>
#include <future>
namespace luna 
{
	namespace assets 
	{

		std::future<stbi_uc*> loadImageAsync(const std::string& filePath) {
			return std::async(std::launch::async, [filePath] {
				int width, height, channels;
				stbi_uc* image = stbi_load(filePath.c_str(), &width, &height, &channels, 4);
				return image;
				});
		}
		ref<asset> textureLoader::loadTexture(assetHandle handle, assetMetadata* metadata)
		{
			textureAssetMetadata* textureMetadata = (textureAssetMetadata*)metadata;

			std::string filePath = reinterpret_cast<char*>(textureMetadata->baseMetaData.filePath);
			filePath += "/";
			filePath += reinterpret_cast<char*>(textureMetadata->baseMetaData.name);

			LN_ERR_FAIL_COND_V_MSG(!std::filesystem::exists(filePath), nullptr, "[ASSETS] could not load asset because asset does no longer exist, filePath: {0}",filePath);

			std::future<stbi_uc*> imageData = loadImageAsync(filePath);

			ref<artemis::allocator> p_allocator = assetLoader::getAllocator();
			artemis::image& image = p_allocator->allocateImage({ textureMetadata->width,textureMetadata->height }, textureMetadata->channels, VK_IMAGE_USAGE_SAMPLED_BIT | VK_IMAGE_USAGE_TRANSFER_DST_BIT);
			artemis::buffer& buffer = p_allocator->allocateBuffer(textureMetadata->imageByteSize, artemis::CPU_COPY, VK_BUFFER_USAGE_STORAGE_TEXEL_BUFFER_BIT | VK_BUFFER_USAGE_TRANSFER_SRC_BIT);

			buffer.setData(imageData.get(), textureMetadata->imageByteSize);
			stbi_image_free(imageData.get());
			p_allocator->copyBufferToImage(buffer, image);
			return std::dynamic_pointer_cast<asset>(createRef<assets::image>(image));
		}

		
	}
}