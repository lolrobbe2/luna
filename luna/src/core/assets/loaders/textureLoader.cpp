#include "textureLoader.h"
#include <core/vulkan/utils/vulkanAllocator.h>
#include <core/vulkan/rendering/vulkanTexture.h>
namespace luna 
{
	namespace assets 
	{
		static void uploadTexture(textureAssetMetadata* textureMetadata,VkBuffer buffer, const VkImage imageHandle, const VkFormat imageFormat)
		{
			std::string filePath = reinterpret_cast<char*>(textureMetadata->baseMetaData.filePath);
			filePath += "/";
			filePath += reinterpret_cast<char*>(textureMetadata->baseMetaData.name);

			if (!std::filesystem::exists(filePath)) return LN_CORE_ERROR("could not find imported file!");

			
			stbi_uc* image = nullptr;
			{
				int width, height, channels;
				stbi_uc* image = stbi_load(filePath.c_str(), &width, &height, &channels, 4);
			}

			void* data = utils::vulkanAllocator::getAllocationInfo((uint64_t)buffer).pMappedData;

			memcpy_s(data, textureMetadata->imageSize, (void*)image, textureMetadata->imageSize);
			stbi_image_free(image);

			utils::vulkanAllocator::uploadTexture(buffer, imageHandle, imageFormat, { textureMetadata->width,textureMetadata->height,textureMetadata->channels }); //TODO threadpool
		}

		ref<asset> textureLoader::loadTexture(assetHandle handle, assetMetadata* metadata)
		{
			textureAssetMetadata* textureMetadata = (textureAssetMetadata*)metadata;

			VkBuffer buffer = VK_NULL_HANDLE;
			VkImage imageHandle = VK_NULL_HANDLE;
			VkImageView imageViewHandle = VK_NULL_HANDLE;

			uint64_t imageSize = textureMetadata->width* textureMetadata->height* textureMetadata->channels;

			utils::vulkanAllocator::createBuffer(&buffer, imageSize, VK_BUFFER_USAGE_STORAGE_TEXEL_BUFFER_BIT | VK_BUFFER_USAGE_TRANSFER_SRC_BIT, VMA_MEMORY_USAGE_CPU_TO_GPU, VMA_ALLOCATION_CREATE_MAPPED_BIT);
			VkFormat imageFormat = utils::vulkanAllocator::getSuitableFormat(VK_BUFFER_USAGE_STORAGE_TEXEL_BUFFER_BIT | VK_BUFFER_USAGE_TRANSFER_SRC_BIT, textureMetadata->channels);
			VkResult result = utils::vulkanAllocator::createImage(&imageHandle, VK_IMAGE_USAGE_SAMPLED_BIT | VK_IMAGE_USAGE_TRANSFER_DST_BIT, VMA_MEMORY_USAGE_GPU_ONLY, { (unsigned int)textureMetadata->width,(unsigned int)textureMetadata->height,1 }, imageFormat);
			
			utils::vulkanAllocator::createImageView(&imageViewHandle, imageHandle, imageFormat, VK_IMAGE_ASPECT_COLOR_BIT); //TODO threadpool

			uploadTexture(textureMetadata, buffer, imageHandle, imageFormat);

			return ref<asset>(new vulkan::vulkanTexture((uint64_t)imageViewHandle,buffer,imageHandle,imageViewHandle,{textureMetadata->width,textureMetadata->height}));
		}
	}
}