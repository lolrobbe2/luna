#include "fontImporter.h"
#include <core/assets/assetImporter.h>
#include <core/artemis/device/allocator.h>
#include <stb_truetype.h>
#include <core/assets/publicTypes/font.h>
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
			glyphscanline scanlines[FONT_ATLAS_ROWS];
		};

		struct imageAtlas
		{
			imageScanline height[FONT_ATLAS_HEIGHT];
		};


		

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
			delete stbtt_GetCodepointBitmap(info, 1, 1, codePoint, &charWidth, &charHeight, newXoff, newYoff);
			*xscale = ((float)GLYPH_WIDTH - 1) / (float)charWidth; //299.0f instead of 300.0f beacuse of floating point "error".
			*yscale = ((float)GLYPH_HEIGHT - 1) / (float)charHeight; //299.0f instead of 300.0f beacuse of floating point "error".
			
			int newCharWidth, newCharHeight;

			return stbtt_GetCodepointBitmap(info, *xscale, *yscale, codePoint, &newCharWidth, &newCharHeight, &xoff, &yoff);
		}

		static void writeGlyphToBuffer(imageAtlas* atlas, scanlineGlyph* glyph, int x, int y)
		{
			int yoff = y * GLYPH_HEIGHT;
			for (size_t i = 0; i < GLYPH_HEIGHT; i++)
			{
				atlas->height[yoff + i].scanlines[x] = glyph->scanlines[i];
			}
			delete glyph;
		}

		static void writeGlyphsIntoBuffer(artemis::buffer& buffer, stbtt_fontinfo* fontInfo, glm::vec2* glypScales, glm::vec2* glyphAdvances)
		{
			LN_PROFILE_FUNCTION();
			imageAtlas* atlas = (imageAtlas*)buffer.getData();
			uint64_t offset = 0;
			for (size_t i = 0; i < 256; i++)
			{

				int index = i - GLYPH_START_INDEX;
				glm::vec2 scale;
				int offsetx, offsety;

				stbi_uc* fontGlyph = createGlyph(fontInfo, i, &scale.x, &scale.y, &offsetx, &offsety);

				if (fontGlyph)
				{
					int y = index / 16;
					int x = index % 16;
					glypScales[i] = (scale);
					glyphAdvances[i] = { offsetx,offsety };

					writeGlyphToBuffer(atlas, (scanlineGlyph*)fontGlyph, x, y);
				}
				else
				{
					glypScales[i] = { 1.0f,1.0f };
					glyphAdvances[i] = { 0.0f,0.0f };
				}
			}
			
			//if (imageHandle != VK_NULL_HANDLE) utils::vulkanAllocator::uploadTexture(*imageBuffer, imageHandle, imageFormat, { FONT_ATLAS_WIDTH,FONT_ATLAS_HEIGHT,1 });
		}


		ref<asset> fontImporter::importFont(assetHandle handle, assetMetadata* metadata)
		{
			LN_PROFILE_FUNCTION();
			fontAssetMetadata* fontMetadata = (fontAssetMetadata*)metadata;
			std::string filePath = reinterpret_cast<char*>(fontMetadata->baseMetaData.filePath);
			filePath += "/";
			filePath += reinterpret_cast<char*>(fontMetadata->baseMetaData.name);

			metadata->fileSizeBytes = std::filesystem::file_size(filePath);

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
				ref<artemis::allocator> p_allocator = assetImporter::getAllocator();

				artemis::image& fontImage = p_allocator->allocateImage({ FONT_ATLAS_WIDTH,FONT_ATLAS_HEIGHT }, 1, VK_IMAGE_USAGE_SAMPLED_BIT | VK_IMAGE_USAGE_TRANSFER_DST_BIT);//createFontTexture(FONT_ATLAS_WIDTH, FONT_ATLAS_HEIGHT, &imageHandle, &imageViewHandle, VK_FORMAT_R8_UNORM);
				
				artemis::buffer& buffer = p_allocator->allocateBuffer(FONT_ATLAS_WIDTH * FONT_ATLAS_HEIGHT, artemis::CPU_ONLY, VK_BUFFER_USAGE_STORAGE_TEXEL_BUFFER_BIT | VK_BUFFER_USAGE_TRANSFER_SRC_BIT);
				
				writeGlyphsIntoBuffer(buffer, &fontInfo, fontMetadata->glyphScales, fontMetadata->glyphAdvances);
				p_allocator->transitionImageLayoutFront(fontImage,VK_IMAGE_LAYOUT_UNDEFINED, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL);
				p_allocator->copyBufferToImage(buffer, fontImage);
				p_allocator->transitionImageLayoutBack(fontImage, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, VK_IMAGE_LAYOUT_GENERAL );

				p_allocator->flush();
				memcpy_s(&fontMetadata->atlas, sizeof(fontAtlas), buffer.getData(), sizeof(fontAtlas));
				fontFile.close();
				return std::dynamic_pointer_cast<assets::asset>(createRef<assets::font>(fontImage,fontMetadata->glyphAdvances));

			}
			else LN_CORE_ERROR("incorrect file format, expected .ttf!");
			fontFile.close();
			return nullptr;		
		}

   }
}

