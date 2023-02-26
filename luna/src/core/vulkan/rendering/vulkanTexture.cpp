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
		vulkanTexture::vulkanTexture(const uint64_t& handle, const glm::vec2& dimensions)
		{
			_handle = handle;
			width = dimensions.x;
			height = dimensions.y;
		}
		vulkanTexture::~vulkanTexture()
		{
			if (!destroy) return;
			utils::vulkanAllocator::destroyImage(imageHandle);
			utils::vulkanAllocator::destroyImageView(imageViewHandle);
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
				VkFormat imageFormat = utils::vulkanAllocator::getSuitableFormat(VK_BUFFER_USAGE_STORAGE_TEXEL_BUFFER_BIT | VK_BUFFER_USAGE_TRANSFER_SRC_BIT, 4);
				VkResult result = utils::vulkanAllocator::createImage(&imageHandle, VK_IMAGE_USAGE_SAMPLED_BIT | VK_IMAGE_USAGE_TRANSFER_DST_BIT, VMA_MEMORY_USAGE_GPU_ONLY, { (unsigned int)width,(unsigned int)height,1 }, imageFormat);
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
		}
		ref<renderer::texture> vulkanFont::getGlyph(char character)
		{
			ref<renderer::texture> glyph = renderer::texture::create(_handle, { 300,300 });
			int index = character - startIndex;
			int yStart = index / 16;
			int xStart = index % 16;
			LN_CORE_INFO("glyph coords: ({0},{1})", xStart, yStart);
			if (!(xStart < 16 && yStart < 16)) return nullptr; //character out of scope;


			
			glm::vec2 uvStart = { (float)xStart / 16,(float)yStart / 16 };
			glm::vec2 uvEnd = { (float)(xStart + 1) / 16,(float)(yStart+1) / 16 };
			glyph->setUv(uvStart, uvEnd);
			glyph->setDestroy(false); //make sure the fontAtlas is not destroyed;
			return glyph;
		}

		glm::vec2 vulkanFont::getAdvance(char character)
		{
			return glm::vec2();
		}
		glm::vec2 vulkanFont::getScale(char character)
		{
			return glypScales[character - startIndex];
		}
		void vulkanFont::createFontTexture() 
		{
			int imageSize = width * height;
			VkFormat imageFormat = utils::vulkanAllocator::getSuitableFormat(VK_BUFFER_USAGE_STORAGE_TEXEL_BUFFER_BIT | VK_BUFFER_USAGE_TRANSFER_SRC_BIT, 0);
			VkResult result = utils::vulkanAllocator::createImage(&imageHandle, VK_IMAGE_USAGE_SAMPLED_BIT | VK_IMAGE_USAGE_TRANSFER_DST_BIT, VMA_MEMORY_USAGE_GPU_ONLY, { (unsigned int)width,(unsigned int)height,1 }, imageFormat);
			utils::vulkanAllocator::createImageView(&imageViewHandle, imageHandle, imageFormat, VK_IMAGE_ASPECT_COLOR_BIT);
			_handle = (uint64_t)imageViewHandle;
		}

		stbi_uc* vulkanFont::createGlyph(const stbtt_fontinfo* info,int codePoint, float* xscale, float* yscale, int* newXoff,int* newYoff)
		{
			int charWidth, charHeight;
			int xoff, yoff;
			
			stbtt_GetCodepointBitmap(info, 1, 1,codePoint , &charWidth, &charHeight, &xoff, &yoff);
			*xscale = 299.0f / (float)charWidth; //299.0f instead of 300.0f beacuse of floating point "error".
			*yscale = 299.0f / (float)charHeight; //299.0f instead of 300.0f beacuse of floating point "error".
			int newCharWidth, newCharHeight;
			
			return stbtt_GetCodepointBitmap(info, *xscale, *yscale, codePoint, &newCharWidth, &newCharHeight, newXoff, newYoff);
		}

		void vulkanFont::writeGlyphsIntoBuffer()
		{
			LN_PROFILE_FUNCTION();
			VkFormat imageFormat = utils::vulkanAllocator::getSuitableFormat(VK_BUFFER_USAGE_STORAGE_TEXEL_BUFFER_BIT | VK_BUFFER_USAGE_TRANSFER_SRC_BIT, 0);
			
			for (size_t i = startIndex; i < 255; i++)
			{
				int index = i - startIndex;
				glm::vec2 scale;
				int offsetx, offsety;
	
				stbi_uc* fontGlyph = createGlyph(&fontInfo, i, &scale.x, &scale.y, &offsetx, &offsety);
				
				if (fontGlyph) {
					int y = index / 16;
					int x = index % 16;
					glypScales.push_back(scale);
					buffer.push_back(VK_NULL_HANDLE);
					utils::vulkanAllocator::createBuffer(&buffer[index], 300 * 300, VK_BUFFER_USAGE_STORAGE_TEXEL_BUFFER_BIT | VK_BUFFER_USAGE_TRANSFER_SRC_BIT, VMA_MEMORY_USAGE_CPU_TO_GPU, VMA_ALLOCATION_CREATE_MAPPED_BIT);
					memcpy(utils::vulkanAllocator::getAllocationInfo((uint64_t)buffer[index]).pMappedData, fontGlyph, sizeof(glyph));
					utils::vulkanAllocator::uploadTexture(this->buffer[index], imageHandle, imageFormat, {300,300,1}, {x * 300,y * 300,0});
				}
				else 
				{
					glypScales.push_back({ 1.0f,1.0f });
					buffer.push_back(VK_NULL_HANDLE);
					LN_CORE_ERROR("could not load glyph: {0}", (char)i);
				}
			}
			utils::vulkanAllocator::flush();
			buffer.resize(0);
		}
	}
}
