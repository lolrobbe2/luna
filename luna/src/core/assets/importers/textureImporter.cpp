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
			LN_PROFILE_FUNCTION();
			textureAssetMetadata* textureMetaData = (textureAssetMetadata*)metadata;
			std::string filePath = reinterpret_cast<char*>(textureMetaData->baseMetaData.filePath);
			filePath += "\\";
			filePath += reinterpret_cast<char*>(textureMetaData->baseMetaData.name);

			metadata->fileSizeBytes = std::filesystem::file_size(filePath);

			VkBuffer buffer = VK_NULL_HANDLE;
			VkImage imageHandle = VK_NULL_HANDLE;
			VkImageView imageViewHandle = VK_NULL_HANDLE;

			if (!std::filesystem::exists(filePath)) { LN_CORE_ERROR("file does not exist: {}", filePath); return ref<asset>(); }

			int width, height, channels;
			stbi_uc* image = stbi_load(filePath.c_str(), &width, &height, &channels, 4);

			if (channels == 3) channels = 4; //RGB formats are most likely not supported! so convert to quad channels.
			
			uint64_t imageSize = width * height * channels;
			/*
			utils::vulkanAllocator::createBuffer(&buffer, imageSize, VK_BUFFER_USAGE_STORAGE_TEXEL_BUFFER_BIT | VK_BUFFER_USAGE_TRANSFER_SRC_BIT, VMA_MEMORY_USAGE_CPU_TO_GPU, VMA_ALLOCATION_CREATE_MAPPED_BIT);
			void* data = utils::vulkanAllocator::getAllocationInfo((uint64_t)buffer).pMappedData;

			
			VkFormat imageFormat = utils::vulkanAllocator::getSuitableFormat(VK_BUFFER_USAGE_STORAGE_TEXEL_BUFFER_BIT | VK_BUFFER_USAGE_TRANSFER_SRC_BIT, channels);
			VkResult result = utils::vulkanAllocator::createImage(&imageHandle, VK_IMAGE_USAGE_SAMPLED_BIT | VK_IMAGE_USAGE_TRANSFER_DST_BIT, VMA_MEMORY_USAGE_GPU_ONLY, { (unsigned int)width,(unsigned int)height,1 }, imageFormat);
			*/
			memcpy_s(data, imageSize, (void*)image, imageSize);
			stbi_image_free(image);
			/*
			utils::vulkanAllocator::createImageView(&imageViewHandle, imageHandle, imageFormat, VK_IMAGE_ASPECT_COLOR_BIT);
			utils::vulkanAllocator::uploadTexture(buffer, imageHandle, imageFormat, { width,height,channels }); //TODO threadpool
			//utils::vulkanAllocator::flush();
			uint64_t _handle = (uint64_t)imageViewHandle;
			*/
			textureMetaData->channels = channels;
			textureMetaData->width = (uint32_t)width;
			textureMetaData->height = (uint32_t)height;

			//return ref<asset>(new vulkan::vulkanTexture(_handle, buffer, imageHandle, imageViewHandle, glm::vec2({ width,height })));
		}
	}
}