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
				if (channels == 3) channels = 4;
				uint64_t imageSize = width * height * channels;
					
				utils::vulkanAllocator::createBuffer(&buffer,imageSize, VK_BUFFER_USAGE_STORAGE_TEXEL_BUFFER_BIT | VK_BUFFER_USAGE_TRANSFER_SRC_BIT, VMA_MEMORY_USAGE_CPU_TO_GPU, VMA_ALLOCATION_CREATE_MAPPED_BIT);
				VkFormat imageFormat = utils::vulkanAllocator::getSuitableFormat(VK_BUFFER_USAGE_STORAGE_TEXEL_BUFFER_BIT | VK_BUFFER_USAGE_TRANSFER_SRC_BIT, channels);
				VkResult result = utils::vulkanAllocator::createImage(&imageHandle, VK_IMAGE_USAGE_SAMPLED_BIT | VK_IMAGE_USAGE_TRANSFER_DST_BIT, VMA_MEMORY_USAGE_GPU_ONLY,{(unsigned int)width,(unsigned int)height,1},imageFormat);
				LN_CORE_INFO("imageCreate result  = {0}", result);
				data = utils::vulkanAllocator::getAllocationInfo((uint64_t)buffer).pMappedData;
				memcpy(data, (void*)image, width * height * channels);
				stbi_image_free(image);
				utils::vulkanAllocator::uploadTexture(buffer, imageHandle,imageFormat, { width,height,channels });
				utils::vulkanAllocator::createImageView(&imageViewHandle, imageHandle, imageFormat, VK_IMAGE_ASPECT_COLOR_BIT);
				_handle = (uint64_t)imageViewHandle;
				this->width = (uint32_t)width;
				this->height = (uint32_t)height;
				textureFile.close();
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
		

		/*----------------------------------------------------------------------------------*/
		/*						vulkan texture atlas implementation							*/
		/*----------------------------------------------------------------------------------*/
		vulkanTextureAtlas::vulkanTextureAtlas(const std::string& filePath, const glm::vec2& texDimensions, const glm::vec2& tileDimensions)
		{
			LN_PROFILE_FUNCTION();
			std::ifstream textureFile(filePath);
			if (textureFile.is_open() && textureFile.good())
			{
				int width, height, channels;
				stbi_uc* image = stbi_load(filePath.c_str(), &width, &height, &channels, 0);
				uint64_t imageSize = width * height * channels;
				utils::vulkanAllocator::createBuffer(&buffer, imageSize, VK_BUFFER_USAGE_STORAGE_TEXEL_BUFFER_BIT | VK_BUFFER_USAGE_TRANSFER_SRC_BIT, VMA_MEMORY_USAGE_CPU_TO_GPU, VMA_ALLOCATION_CREATE_MAPPED_BIT);
				VkFormat imageFormat = utils::vulkanAllocator::getSuitableFormat(VK_BUFFER_USAGE_STORAGE_TEXEL_BUFFER_BIT | VK_BUFFER_USAGE_TRANSFER_SRC_BIT, 4);
				VkResult result = utils::vulkanAllocator::createImage(&imageHandle, VK_IMAGE_USAGE_SAMPLED_BIT | VK_IMAGE_USAGE_TRANSFER_DST_BIT, VMA_MEMORY_USAGE_GPU_ONLY, { (unsigned int)width,(unsigned int)height,1 }, imageFormat);
				LN_CORE_INFO("imageCreate result  = {0}", result);
				data = utils::vulkanAllocator::getAllocationInfo((uint64_t)buffer).pMappedData;
				memcpy(data, (void*)image, width * height * 4);
				stbi_image_free(image);
				utils::vulkanAllocator::uploadTexture(buffer, imageHandle, imageFormat, { width,height,channels });
				utils::vulkanAllocator::createImageView(&imageViewHandle, imageHandle, imageFormat, VK_IMAGE_ASPECT_COLOR_BIT);
				_handle = (uint64_t)imageViewHandle;

				textureFile.close();
				return;
			}
			LN_CORE_CRITICAL("could not open texture file at: {0}", filePath);
		}

		vulkanTextureAtlas::vulkanTextureAtlas(const std::string& filePath, const glm::vec2& texDimensions)
		{
			LN_PROFILE_FUNCTION();
			std::ifstream textureFile(filePath);
			if (textureFile.is_open() && textureFile.good())
			{
				int width, height, channels;
				stbi_uc* image = stbi_load(filePath.c_str(), &width, &height, &channels, 0);
				uint64_t imageSize = width * height * channels;
				utils::vulkanAllocator::createBuffer(&buffer, imageSize, VK_BUFFER_USAGE_STORAGE_TEXEL_BUFFER_BIT | VK_BUFFER_USAGE_TRANSFER_SRC_BIT, VMA_MEMORY_USAGE_CPU_TO_GPU, VMA_ALLOCATION_CREATE_MAPPED_BIT);
				VkFormat imageFormat = utils::vulkanAllocator::getSuitableFormat(VK_BUFFER_USAGE_STORAGE_TEXEL_BUFFER_BIT | VK_BUFFER_USAGE_TRANSFER_SRC_BIT, 4);
				VkResult result = utils::vulkanAllocator::createImage(&imageHandle, VK_IMAGE_USAGE_SAMPLED_BIT | VK_IMAGE_USAGE_TRANSFER_DST_BIT, VMA_MEMORY_USAGE_GPU_ONLY, { (unsigned int)width,(unsigned int)height,1 }, imageFormat);
				LN_CORE_INFO("imageCreate result  = {0}", result);
				data = utils::vulkanAllocator::getAllocationInfo((uint64_t)buffer).pMappedData;
				memcpy(data, (void*)image, width * height * channels);
				stbi_image_free(image);
				utils::vulkanAllocator::uploadTexture(buffer, imageHandle, imageFormat, { width,height,channels });
				utils::vulkanAllocator::createImageView(&imageViewHandle, imageHandle, imageFormat, VK_IMAGE_ASPECT_COLOR_BIT);
				_handle = (uint64_t)imageViewHandle;
				this->width = (uint32_t)width;
				this->height = (uint32_t)height;
				textureFile.close();
				return;
			}
			LN_CORE_CRITICAL("could not open texture file at: {0}", filePath);
		}

		uint32_t vulkanTextureAtlas::getTileWidth()
		{
			if (tileCustomTexCoords.size()) return uint32_t();
			return uint32_t();
		}

		uint32_t vulkanTextureAtlas::getTileWidth(const uint16_t& xIndex, const uint16_t& yIndex)
		{
			if (!(tileWidths || tileHeight)) return uint32_t();
			if (xIndex > tileCustomTexCoords.size() && yIndex > tileCustomTexCoords[xIndex].size()) return uint32_t();
			return tileCustomTexCoords[xIndex][yIndex].x;
		}

		uint32_t vulkanTextureAtlas::getTileHeight()
		{
			if (tileCustomTexCoords.size()) return uint32_t();
			return uint32_t();
		}

		uint32_t vulkanTextureAtlas::getTileHeight(const uint16_t& xIndex, const uint16_t& yIndex)
		{
			if (!(tileWidths || tileHeight)) return uint32_t();
			if (xIndex > tileCustomTexCoords.size() && yIndex > tileCustomTexCoords[xIndex].size()) return uint32_t();
			return tileCustomTexCoords[xIndex][yIndex].y;
		}

		glm::vec2 vulkanTextureAtlas::addTile(const glm::vec2& dimensions)
		{

			return addTile(dimensions.x, dimensions.y);
		}

		glm::vec2 vulkanTextureAtlas::addTile(const uint32_t& width, const uint32_t& height)
		{
			//model 
			//horizontal stripes consisting of free blocks 
			//1) start traverse
			//2) check stripe for free texture area.
			//3) crop free block.
			//4) adjust stripe.
			for (auto& stripe : atlas)
			{
				vulkanTextureAtlas::freeStripeBlock* foundStripe = checkStripe(stripe, width, height);
				if (foundStripe) 
				{
					if (foundStripe->getWidth() != width) {
						//TODO
					}
				}
			}

			return glm::vec2();
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

		vulkanTextureAtlas::freeStripeBlock* vulkanTextureAtlas::checkStripe(stripe& stripe, const uint32_t& width, const uint32_t& height)
		{
			for (vulkanTextureAtlas::freeStripeBlock& block : stripe.stripeVector)
			{
				if(block.getWidth() >= width && block.getHeight() >= height) return &block; //search hit
			}
			return nullptr; //search miss
		}

		/*-------------------------------------------------------------------------------------*/


		vulkanFont::vulkanFont(const std::string& filePath)
		{
			LN_PROFILE_FUNCTION();
			std::ifstream fontFile(filePath, std::ios::binary);
			if (fontFile.is_open() && fontFile.good())
			{
				LN_CORE_TRACE("succesfuly loaded fontFile! {0}",filePath);
				std::vector<unsigned char> buffer(std::istreambuf_iterator<char>(fontFile), {});
				if(stbtt_InitFont(&fontInfo,buffer.data(), 0))
				{
					LN_CORE_TRACE("init font succesful");
				}
				fontFile.close();
			}
		}
		ref<renderer::texture> vulkanFont::getGlyph(char character)
		{
			return nullptr;
		}

		glm::vec2 vulkanFont::getAdvance(char character)
		{
			return glm::vec2();
		}
	}
}
