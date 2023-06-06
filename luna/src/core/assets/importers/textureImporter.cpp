#include "textureImporter.h"
#include <core/vulkan/utils/vulkanAllocator.h>
#include <core/vulkan/device/vulkanDevice.h>
#include <core/vulkan/rendering/vulkanTexture.h>
namespace luna
{
	namespace assets 
	{

		ref<asset> textureImporter::importTexture(assetHandle handle, assetMetadata* metadata)
		{
			textureAssetMetaData* textureMetaData = (textureAssetMetaData*)metadata;
			std::string filePath = reinterpret_cast<char*>(textureMetaData->baseMetaData.filePath);
			filePath += reinterpret_cast<char*>(textureMetaData->baseMetaData.filePath);
			std::ifstream textureFile(filePath);

			VkBuffer buffer = VK_NULL_HANDLE;
			VkImage imageHandle = VK_NULL_HANDLE;
			VkImageView imageViewHandle = VK_NULL_HANDLE;

			if (textureFile.is_open() && textureFile.good())
			{
				int width, height, channels;
				stbi_uc* image = stbi_load(filePath.c_str(), &width, &height, &channels, 4);
				if (channels == 3) channels = 4; //RGB formats are most likely not supported! so convert to quad channels.
				uint64_t imageSize = width * height * channels;

				utils::vulkanAllocator::createBuffer(&buffer, imageSize, VK_BUFFER_USAGE_STORAGE_TEXEL_BUFFER_BIT | VK_BUFFER_USAGE_TRANSFER_SRC_BIT, VMA_MEMORY_USAGE_CPU_TO_GPU, VMA_ALLOCATION_CREATE_MAPPED_BIT);
				VkFormat imageFormat = utils::vulkanAllocator::getSuitableFormat(VK_BUFFER_USAGE_STORAGE_TEXEL_BUFFER_BIT | VK_BUFFER_USAGE_TRANSFER_SRC_BIT, channels);
				VkResult result = utils::vulkanAllocator::createImage(&imageHandle, VK_IMAGE_USAGE_SAMPLED_BIT | VK_IMAGE_USAGE_TRANSFER_DST_BIT, VMA_MEMORY_USAGE_GPU_ONLY, { (unsigned int)width,(unsigned int)height,1 }, imageFormat);
				void* data = utils::vulkanAllocator::getAllocationInfo((uint64_t)buffer).pMappedData;
				memcpy_s(data, width * height * channels, (void*)image, width * height * channels);
				stbi_image_free(image);
				utils::vulkanAllocator::uploadTexture(buffer, imageHandle, imageFormat, { width,height,channels });
				utils::vulkanAllocator::createImageView(&imageViewHandle, imageHandle, imageFormat, VK_IMAGE_ASPECT_COLOR_BIT); //TODO threadpool
				uint64_t _handle = (uint64_t)imageViewHandle;

				textureMetaData->channels = channels;
				textureMetaData->width = (uint32_t)width;
				textureMetaData->height = (uint32_t)height;
				textureFile.close();

				ref<renderer::texture> texture = ref<renderer::texture>(new vulkan::vulkanTexture(_handle, buffer, imageHandle, imageViewHandle, glm::vec2({ width,height })));

				return std::dynamic_pointer_cast<asset>(texture);
			}
			return ref<asset>();
		}
	}
}