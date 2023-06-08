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
				if (channels == 3) channels = 4; //RGB formats are most likely not supported! so convert to quad channels.
				uint64_t imageSize = width * height * channels;
					
				utils::vulkanAllocator::createBuffer(&buffer,imageSize, VK_BUFFER_USAGE_STORAGE_TEXEL_BUFFER_BIT | VK_BUFFER_USAGE_TRANSFER_SRC_BIT, VMA_MEMORY_USAGE_CPU_TO_GPU, VMA_ALLOCATION_CREATE_MAPPED_BIT);
				VkFormat imageFormat = utils::vulkanAllocator::getSuitableFormat(VK_BUFFER_USAGE_STORAGE_TEXEL_BUFFER_BIT | VK_BUFFER_USAGE_TRANSFER_SRC_BIT, channels);
				VkResult result = utils::vulkanAllocator::createImage(&imageHandle, VK_IMAGE_USAGE_SAMPLED_BIT | VK_IMAGE_USAGE_TRANSFER_DST_BIT, VMA_MEMORY_USAGE_GPU_ONLY,{(unsigned int)width,(unsigned int)height,1},imageFormat);
				data = utils::vulkanAllocator::getAllocationInfo((uint64_t)buffer).pMappedData;
				memcpy_s(data, width * height * channels, (void*)image, width * height * channels);
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
		vulkanTexture::vulkanTexture(const uint64_t& handle, const glm::vec2& dimensions)
		{
			_handle = handle;
			width = dimensions.x;
			height = dimensions.y;
		}
		vulkanTexture::vulkanTexture(uint64_t handle, VkBuffer buffer, VkImage imageHandle, VkImageView imageViewHandle, const glm::vec2& dimensions) : buffer(buffer),imageHandle(imageHandle),imageViewHandle(imageViewHandle)
		{
			width = dimensions.x;
			height = dimensions.y; 
			_handle = handle;
		}
		vulkanTexture::~vulkanTexture()
		{
			if (!destroy) return;
			utils::vulkanAllocator::destroyImageView(imageViewHandle);
			utils::vulkanAllocator::destroyImage(imageHandle);
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

		assets::assetType vulkanTexture::getType() const
		{
			return assets::assetType::texture;
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
				VkFormat imageFormat = utils::vulkanAllocator::getSuitableFormat(VK_BUFFER_USAGE_STORAGE_TEXEL_BUFFER_BIT | VK_BUFFER_USAGE_TRANSFER_SRC_BIT, 4);
				VkResult result = utils::vulkanAllocator::createImage(&imageHandle, VK_IMAGE_USAGE_SAMPLED_BIT | VK_IMAGE_USAGE_TRANSFER_DST_BIT, VMA_MEMORY_USAGE_GPU_ONLY, { (unsigned int)width,(unsigned int)height,1 }, imageFormat);
				data = utils::vulkanAllocator::getAllocationInfo((uint64_t)buffer).pMappedData;
				memcpy_s(data,width * height * 4, (void*)image, width * height * 4);
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
				data = utils::vulkanAllocator::getAllocationInfo((uint64_t)buffer).pMappedData;
				memcpy_s(data, width * height * 4, (void*)image, width * height * channels);
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
			LN_CORE_WARN("deprecated");
			/*
			std::ifstream fontFile(filePath, std::ios::binary);
			if (fontFile.is_open() && fontFile.good())
			{
				
				std::vector<unsigned char> buffer(std::istreambuf_iterator<char>(fontFile), {});
				
				
				if (stbtt_InitFont(&fontInfo, buffer.data(), 0))
				{
					createFontTexture();
					writeGlyphsIntoBuffer();
					LN_CORE_TRACE("succesfuly loaded fontFile! {0}", filePath);
				}
				else LN_CORE_ERROR("incorrect file format, expected .ttf!");
				fontFile.close();
			}
			*/
		}
		vulkanFont::vulkanFont(VkBuffer imageBuffer, VkImage imageHandle, VkImageView imageViewHandle, glm::vec2* glyphScales, glm::vec2* glyphAdvances) : imageBuffer(imageBuffer),imageHandle(imageHandle),imageViewHandle(imageViewHandle)
		{
			memcpy_s((void*)this->glyphScales, FONT_ATLAS_GLYPH_AMOUNT * sizeof(glm::vec2), glyphScales, FONT_ATLAS_GLYPH_AMOUNT * sizeof(glm::vec2));
			memcpy_s((void*)this->glyphAdvances, FONT_ATLAS_GLYPH_AMOUNT * sizeof(glm::vec2), glyphAdvances, FONT_ATLAS_GLYPH_AMOUNT * sizeof(glm::vec2));
			_handle = (uint64_t)imageViewHandle;
		}
		vulkanFont::~vulkanFont() 
		{
			utils::vulkanAllocator::destroyImageView(imageViewHandle );
			utils::vulkanAllocator::destroyImage(imageHandle);
		}
		ref<renderer::texture> vulkanFont::getGlyph(char character)
		{
			LN_PROFILE_FUNCTION();

			//GLYPH_WIDTH / getScale(character).x  (normalizing the textures 0.0f-1.0f)

			ref<renderer::texture> glyph = renderer::texture::create(_handle, { GLYPH_WIDTH / getScale(character).x,GLYPH_HEIGHT / getScale(character).y });
			int index = character - startIndex;
			int yStart = index / FONT_ATLAS_COLUMNS;
			int xStart = index % FONT_ATLAS_ROWS;
			if (!(xStart < FONT_ATLAS_ROWS && yStart < FONT_ATLAS_COLUMNS)) return nullptr; //character out of scope;
			glm::vec2 uvStart = { (float)xStart / FONT_ATLAS_ROWS,(float)yStart / FONT_ATLAS_COLUMNS };
			glm::vec2 uvEnd = { (float)(xStart + 1) / FONT_ATLAS_ROWS,(float)(yStart+1) / FONT_ATLAS_COLUMNS };
			glyph->setUv(uvStart, uvEnd);
			glyph->setDestroy(false); //make sure the fontAtlas is not destroyed;
			return glyph;
		}
		
		glm::vec2 vulkanFont::getAdvance(char character)
		{
			return glyphAdvances[character];
		}
		glm::vec2 vulkanFont::getScale(char character)
		{
			return glyphScales[character];
		}
		assets::assetType vulkanFont::getType() const
		{
			return assets::assetType::font;
		}
	}
}
