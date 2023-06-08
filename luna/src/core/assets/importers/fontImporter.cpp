#include "fontImporter.h"
#include <core/vulkan/utils/vulkanAllocator.h>
#include <core/vulkan/device/vulkanDevice.h>
#include <core/vulkan/rendering/vulkanTexture.h>
namespace luna 
{
    namespace assets
    {
		struct glyph
		{
			stbi_uc _glyph[GLYPH_WIDTH * GLYPH_HEIGHT];
		};
		
		struct glyphscanline
		{
			stbi_uc scanLine[GLYPH_WIDTH];
		};

		struct scanlineGlyph
		{
			glyphscanline scanlines[GLYPH_HEIGHT];
		};


		struct imageScanline
		{
			stbi_uc scanline[FONT_ATLAS_WIDTH];
		};



		/**
		 * @brief allocates and creates the atlas texture from wich glyph can be sampled from.
		 *
		 */
		static void createFontTexture(int width, int height, VkImage* imageHandle, VkImageView* imageViewHandle, VkFormat imageFormat)
		{
			int imageSize = width * height;
			VkResult result = utils::vulkanAllocator::createImage(imageHandle, VK_IMAGE_USAGE_SAMPLED_BIT | VK_IMAGE_USAGE_TRANSFER_DST_BIT, VMA_MEMORY_USAGE_GPU_ONLY, { (unsigned int)width,(unsigned int)height,1 }, imageFormat);
			utils::vulkanAllocator::createImageView(imageViewHandle, *imageHandle, imageFormat, VK_IMAGE_ASPECT_COLOR_BIT);
		}

		/*
		 * @brief creates a font glyph and gurantees the texture height and width to be 300 by 300 pixels.
		 *
		 * \param const stbtt_fontinfo* info
		 * \param int codePoint (char code to create glyph from)
		 * \param float* xscale: relative scale to get 1.0f width back. (300 / xscale)
		 * \param float* yscale: relative scale to get 1.0f height back. (300 / yscale)
		 * \param int* newXoff: relative xoffset.
		 * \param newYoff: relative yoffset.
		 * \return stbi_uc* pointer to texure data.
		 */
		static stbi_uc* createGlyph(const stbtt_fontinfo* info, int codePoint, float* xscale, float* yscale, int* newXoff, int* newYoff)
		{
			LN_PROFILE_FUNCTION();
			int xoff, yoff;
			
			int charWidth, charHeight;
			stbtt_GetCodepointBitmap(info, 1, 1, codePoint, &charWidth, &charHeight, newXoff, newYoff);
			*xscale = ((float)GLYPH_WIDTH - 1) / (float)charWidth; //299.0f instead of 300.0f beacuse of floating point "error".
			*yscale = ((float)GLYPH_HEIGHT - 1) / (float)charHeight; //299.0f instead of 300.0f beacuse of floating point "error".
			
			int newCharWidth, newCharHeight;

			return stbtt_GetCodepointBitmap(info, *xscale, *yscale, codePoint, &newCharWidth, &newCharHeight, &xoff, &yoff);
		}

		static void writeGlyphsIntoBuffer(VkBuffer* imageBuffer, VkImage& imageHandle, const stbtt_fontinfo* fontInfo, glm::vec2* glypScales, glm::vec2* glyphAdvances, VkFormat imageFormat)
		{
			LN_PROFILE_FUNCTION();
			utils::vulkanAllocator::createBuffer(imageBuffer, FONT_ATLAS_WIDTH * FONT_ATLAS_HEIGHT, VK_BUFFER_USAGE_STORAGE_TEXEL_BUFFER_BIT | VK_BUFFER_USAGE_TRANSFER_SRC_BIT, VMA_MEMORY_USAGE_CPU_TO_GPU, VMA_ALLOCATION_CREATE_MAPPED_BIT);
			size_t bufferSize = utils::vulkanAllocator::getAllocationInfo((uint64_t)*imageBuffer).size;
			void* bufferBase = utils::vulkanAllocator::getAllocationInfo((uint64_t)*imageBuffer).pMappedData;
			glyph* bufferPtr = (glyph*)bufferBase;
			uint64_t offset = 0;
			for (size_t i = 0; i < 256; i++)
			{

				int index = i - GLYPH_START_INDEX;
				glm::vec2 scale;
				int offsetx, offsety;

				stbi_uc* fontGlyph = createGlyph(fontInfo, i, &scale.x, &scale.y, &offsetx, &offsety);

				if (fontGlyph)
				{
					int y = index / FONT_ATLAS_COLUMNS;
					int x = index % FONT_ATLAS_ROWS;
					glypScales[i] = (scale);
					glyphAdvances[i] = { offsetx,offsety };
					memcpy_s(bufferPtr, bufferSize, fontGlyph, sizeof(glyph));
					bufferPtr++;
					if (imageHandle != VK_NULL_HANDLE) utils::vulkanAllocator::uploadTexture(*imageBuffer, imageHandle, imageFormat, { GLYPH_WIDTH,GLYPH_HEIGHT,1 }, { x * GLYPH_WIDTH,y * GLYPH_HEIGHT,0 }, { GLYPH_WIDTH,GLYPH_HEIGHT }, offset);
					offset += sizeof(glyph);
				}
				else
				{
					glypScales[i] = { 1.0f,1.0f };
					glyphAdvances[i] = { 0.0f,0.0f };
					bufferPtr++;
					offset += sizeof(glyph);
				}
			}
			utils::vulkanAllocator::flush();
			//utils::vulkanAllocator::downloadTexture()
		}

		static void writeGlyphToBuffer(imageScanline* baseptr, scanlineGlyph* glyph, int x, int y)
		{
			//step 1 find first top left point
			//step 2 memcpy_s
			//step 3 advance line pointer by FONT_ATLAS_WIDTH
			//step 4 goto 2 (repeat)
			int bufferOffset = GET_FONT_BUFFER_OFFSET(x, y);
			imageScanline* offsetptr = baseptr;
			offsetptr += bufferOffset;

			for (size_t i = 0; i < GLYPH_HEIGHT; i++)
			{
				memcpy_s(&baseptr->scanline[i], sizeof(glyphscanline), &glyph->scanlines[i], sizeof(glyphscanline));
			}
		}

		static void* writeGlyphsIntoBuffer2(VkBuffer* imageBuffer, VkImage& imageHandle, stbtt_fontinfo* fontInfo, glm::vec2* glypScales, glm::vec2* glyphAdvances, VkFormat imageFormat)
		{
			LN_PROFILE_FUNCTION();
			utils::vulkanAllocator::createBuffer(imageBuffer, FONT_ATLAS_WIDTH * FONT_ATLAS_HEIGHT, VK_BUFFER_USAGE_STORAGE_TEXEL_BUFFER_BIT | VK_BUFFER_USAGE_TRANSFER_SRC_BIT, VMA_MEMORY_USAGE_CPU_TO_GPU, VMA_ALLOCATION_CREATE_MAPPED_BIT);
			size_t bufferSize = utils::vulkanAllocator::getAllocationInfo((uint64_t)*imageBuffer).size;
			void* bufferBase = utils::vulkanAllocator::getAllocationInfo((uint64_t)*imageBuffer).pMappedData;
			imageScanline* bufferPtr = (imageScanline*)bufferBase;
			uint64_t offset = 0;
			for (size_t i = 0; i < 256; i++)
			{

				int index = i - GLYPH_START_INDEX;
				glm::vec2 scale;
				int offsetx, offsety;

				stbi_uc* fontGlyph = createGlyph(fontInfo, i, &scale.x, &scale.y, &offsetx, &offsety);

				if (fontGlyph)
				{
					int y = index / FONT_ATLAS_COLUMNS;
					int x = index % FONT_ATLAS_ROWS;
					glypScales[i] = (scale);
					glyphAdvances[i] = { offsetx,offsety };
					writeGlyphToBuffer(bufferPtr, (scanlineGlyph*)fontGlyph, x, y);
				}
				else
				{
					glypScales[i] = { 1.0f,1.0f };
					glyphAdvances[i] = { 0.0f,0.0f };
				}
			}
			if (imageHandle != VK_NULL_HANDLE) utils::vulkanAllocator::uploadTexture(*imageBuffer, imageHandle, imageFormat, { FONT_ATLAS_WIDTH,FONT_ATLAS_HEIGHT,1 });
			utils::vulkanAllocator::flush();
			return bufferBase;
		}


		ref<asset> fontImporter::importFont(assetHandle handle, assetMetadata* metadata)
		{
			LN_PROFILE_FUNCTION();
			fontAssetMetadata* fontMetadata = (fontAssetMetadata*)metadata;
			std::string filePath = reinterpret_cast<char*>(fontMetadata->baseMetaData.filePath);
			filePath += "/";
			filePath += reinterpret_cast<char*>(fontMetadata->baseMetaData.name);

			stbtt_fontinfo fontInfo;

			VkBuffer imageBuffer = VK_NULL_HANDLE;
			VkImage imageHandle = VK_NULL_HANDLE;
			VkImageView imageViewHandle = VK_NULL_HANDLE;
			std::ifstream fontFile(filePath, std::ios::in | std::ios::binary);
			if (!(fontFile.is_open() && fontFile.good())) { LN_CORE_WARN("could not find file at path: {}", filePath); return ref<asset>(); }
			std::vector<uint8_t> buffer((std::istreambuf_iterator<char>(fontFile)), std::istreambuf_iterator<char>());


			LN_CORE_INFO("loading font at absolute path of: {}", std::filesystem::absolute(std::filesystem::path(filePath)));

			if (stbtt_InitFont(&fontInfo, buffer.data(), 0))
			{
				VkFormat imageFormat = utils::vulkanAllocator::getSuitableFormat(VK_BUFFER_USAGE_STORAGE_TEXEL_BUFFER_BIT | VK_BUFFER_USAGE_TRANSFER_SRC_BIT, 0);

				createFontTexture(FONT_ATLAS_WIDTH, FONT_ATLAS_HEIGHT, &imageHandle, &imageViewHandle, imageFormat);
				void* data = writeGlyphsIntoBuffer2(&imageBuffer, imageHandle, &fontInfo, fontMetadata->glyphScales, fontMetadata->glyphAdvances, imageFormat);
				
				memcpy_s(&fontMetadata->atlas, sizeof(fontAtlas), data, sizeof(fontAtlas));
				LN_CORE_TRACE("succesfuly loaded fontFile! {}", filePath);
			}
			else LN_CORE_ERROR("incorrect file format, expected .ttf!");
			fontFile.close();
			return ref<asset>(new vulkan::vulkanFont(imageBuffer, imageHandle, imageViewHandle, fontMetadata->glyphScales, fontMetadata->glyphAdvances));
		}

   }
}

