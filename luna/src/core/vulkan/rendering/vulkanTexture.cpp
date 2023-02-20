#include "vulkanTexture.h"
#include <core/vulkan/utils/vulkanAllocator.h>
#include <core/vulkan/device/vulkanDevice.h>
#include <lnpch.h>
namespace luna
{
	namespace vulkan
	{
		vulkanTexture::vulkanTexture(const std::string& filePath)
		{
			LN_PROFILE_FUNCTION();
			std::ifstream textureFile(filePath);
			if(textureFile.is_open() && textureFile.good())
			{
				int width, height, channels;
				stbi_uc* image =  stbi_load(filePath.c_str(), &width, &height, &channels, 4);
				uint64_t imageSize = width * height * 4;
				//imageSize += imageSize % 16;
				utils::vulkanAllocator::createBuffer(&buffer,imageSize, VK_BUFFER_USAGE_STORAGE_TEXEL_BUFFER_BIT | VK_BUFFER_USAGE_TRANSFER_SRC_BIT, VMA_MEMORY_USAGE_CPU_TO_GPU, VMA_ALLOCATION_CREATE_MAPPED_BIT);
				VkFormat imageFormat = utils::vulkanAllocator::getSuitableFormat(VK_BUFFER_USAGE_STORAGE_TEXEL_BUFFER_BIT | VK_BUFFER_USAGE_TRANSFER_SRC_BIT, 4);
				VkResult result = utils::vulkanAllocator::createImage(&imageHandle, VK_IMAGE_USAGE_SAMPLED_BIT | VK_IMAGE_USAGE_TRANSFER_DST_BIT, VMA_MEMORY_USAGE_GPU_ONLY,{(unsigned int)width,(unsigned int)height,1},imageFormat);
				LN_CORE_INFO("imageCreate result  = {0}", result);
				data = utils::vulkanAllocator::getAllocationInfo((uint64_t)buffer).pMappedData;
				memcpy(data, (void*)image, width * height * 4);
				stbi_image_free(image);
				utils::vulkanAllocator::uploadTexture(buffer, imageHandle,imageFormat, { width,height,channels });
				utils::vulkanAllocator::createImageView(&imageViewHandle, imageHandle, imageFormat, VK_IMAGE_ASPECT_COLOR_BIT);
				_handle = (uint64_t)imageViewHandle;
				return;
			}
			LN_CORE_CRITICAL("could not open texture file at: {0}", filePath);
		}
		vulkanTexture::vulkanTexture(void* texelData, const glm::vec2& dimensions)
		{
			LN_PROFILE_FUNCTION();
			utils::vulkanAllocator::createBuffer(&buffer, dimensions.x * dimensions.y, VkBufferUsageFlagBits::VK_BUFFER_USAGE_STORAGE_TEXEL_BUFFER_BIT, VMA_MEMORY_USAGE_GPU_ONLY, VmaAllocationCreateFlagBits::VMA_ALLOCATION_CREATE_WITHIN_BUDGET_BIT);
			utils::vulkanAllocator::createImage(&imageHandle, VkImageUsageFlagBits::VK_IMAGE_USAGE_SAMPLED_BIT, VMA_MEMORY_USAGE_GPU_ONLY, { (unsigned int)width,(unsigned int)height,0 }, VK_FORMAT_R8G8B8A8_SRGB);
			setData(texelData,dimensions.x * dimensions.y * 4);
			LN_CORE_INFO("not fully implemented");
		}
		vulkanTexture::~vulkanTexture()
		{
			//utils::vulkanAllocator::destroyImage(imageHandle);
		}
		uint32_t vulkanTexture::getWidth() const
		{
			return width;
		}
		uint32_t vulkanTexture::getHeight() const
		{
			return height;
		}
		const std::string& vulkanTexture::getPath() const
		{
			// TODO: insert return statement here
			return "";
		}
		void vulkanTexture::setData(void* data, uint32_t size)
		{

		}
		void vulkanTexture::bind(uint32_t slot) const
		{

		}
		bool vulkanTexture::isLoaded() const
		{
			return false;
		}

		stbi_uc* vulkanTexture::reformat(stbi_uc* src, int& srcChannels,const uint32_t& dstChannels,const uint32_t& width,const uint32_t&  height)
		{
			if (srcChannels >= dstChannels) return src;
			
			std::vector<stbi_uc> destination;


			for (size_t i = 0; i < width * height; i++)
			{
				for (size_t x = 0; x < srcChannels; x++) destination.push_back(src[i]);
				for (size_t x = srcChannels; x < dstChannels; x++)
				{
					if (x == 3)destination.push_back(1);
					else destination.push_back(0);
				}
			}
			//stbi_image_free(src);
			srcChannels = dstChannels;
			return destination.data();
		}
		
		vulkanTextureAtlas::vulkanTextureAtlas(const std::string& filePath, const glm::vec2& texDimensions, const glm::vec2& tileDimensions)
		{
		}

		vulkanTextureAtlas::vulkanTextureAtlas(const std::string& filePath, const glm::vec2& texDimensions)
		{
		}

		uint32_t vulkanTextureAtlas::getTileWidth()
		{
			return uint32_t();
		}

		uint32_t vulkanTextureAtlas::getTileWidth(const uint16_t& xIndex, const uint16_t& yIndex)
		{
			return uint32_t();
		}

		uint32_t vulkanTextureAtlas::getTileHeight()
		{
			return uint32_t();
		}

		uint32_t vulkanTextureAtlas::getTileHeight(const uint16_t& xIndex, const uint16_t& yIndex)
		{
			return uint32_t();
		}

		uint32_t vulkanTextureAtlas::addTile(const glm::vec2& dimensions)
		{
			return uint32_t();
		}

		uint32_t vulkanTextureAtlas::addTile(const uint32_t& width, const uint32_t& height)
		{
			return uint32_t();
		}

		glm::vec2 vulkanTextureAtlas::getTileDimensions(const glm::vec2& textureindex)
		{
			return glm::vec2();
		}

		glm::vec2 vulkanTextureAtlas::getTextureUv(const glm::vec2& textureindex)
		{
			return glm::vec2();
		}

		ref<renderer::texture> vulkanTextureAtlas::getTileAsTexture(const glm::vec2& textureindex)
		{
			return ref<renderer::texture>();
		}

	}
}
