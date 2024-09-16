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
		static stbi_uc* createGlyph(const stbtt_fontinfo* info, int codePoint, float* xscale, float* yscale, int* newXoff, int* newYoff, int* advanceWidth, int* leftSideBearing)
		{
			LN_PROFILE_FUNCTION();
			int xoff, yoff;
			
			int charWidth, charHeight;
			delete stbtt_GetCodepointBitmap(info, 1, 1, codePoint, &charWidth, &charHeight, newXoff, newYoff);
			*xscale = ((float)GLYPH_WIDTH - 1) / (float)charWidth; //299.0f instead of 300.0f beacuse of floating point "error".
			*yscale = ((float)GLYPH_HEIGHT - 1) / (float)charHeight; //299.0f instead of 300.0f beacuse of floating point "error".
			
			int newCharWidth, newCharHeight;
			/**
			 * @brief The advance width, which determines how much to move the cursor
			 *        after drawing the character.
			 *
			 * The advance width is the h	orizontal distance to move the cursor to the
			 * next character's position after rendering this character. It is the
			 * distance between the current character's origin and the next character's
			 * origin in a horizontal layout.
			 */

			 /**
			  * @brief The left side bearing, which is the distance from the current
			  *        cursor position to the left edge of the character.
			  *
			  * The left side bearing defines the space between the start of the
			  * character's bounding box and the current cursor position. It can be
			  * positive (indicating some space) or negative (indicating an overlap).
			  */
			stbtt_GetCodepointHMetrics(info, codePoint, advanceWidth, leftSideBearing);
			return stbtt_GetCodepointBitmap(info, *xscale, *yscale, codePoint, &newCharWidth, &newCharHeight, &xoff, &yoff);
		}

		static void writeGlyphToBuffer(imageAtlas* atlas, scanlineGlyph* glyph, int x, int y)
		{
			int yoff = y * GLYPH_HEIGHT;
			LN_UNROLL_LOOP
			for (size_t i = 0; i < GLYPH_HEIGHT; i++)
			{
				atlas->height[yoff + i].scanlines[x] = glyph->scanlines[i];
			}
			delete glyph;
		}

		static void writeGlyphsIntoBuffer(artemis::buffer& buffer, stbtt_fontinfo* fontInfo, glm::vec2* glypScales, glm::vec2* glyphOffests,glm::vec2* glyphAdvances)
		{
			LN_PROFILE_FUNCTION();
			imageAtlas* atlas = (imageAtlas*)buffer.getData();
			uint64_t offset = 0;
			LN_UNROLL_LOOP
			for (size_t i = 0; i < 256; i++)
			{
				float advanceScale = stbtt_ScaleForPixelHeight(fontInfo, 100);
				int index = i - GLYPH_START_INDEX;
				glm::vec2 scale;
				int offsetx, offsety;
				int advanceWidth, leftSideBearing;


				stbi_uc* fontGlyph = createGlyph(fontInfo, i, &scale.x, &scale.y, &offsetx, &offsety,&advanceWidth,&leftSideBearing);

				if (fontGlyph)
				{
					int y = index / 16;
					int x = index % 16;
					glypScales[i] = (scale);
					glyphOffests[i] = { offsetx,offsety };
					glyphAdvances[i] = { advanceWidth * advanceScale,leftSideBearing * advanceScale};	

					writeGlyphToBuffer(atlas, (scanlineGlyph*)fontGlyph, x, y);
				}
				else
				{
					glypScales[i] = { 1.0f,1.0f };
					glyphOffests[i] = { 0.0f,0.0f };
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
				
				writeGlyphsIntoBuffer(buffer, &fontInfo, fontMetadata->glyphScales, fontMetadata->glyphOffests,fontMetadata->glyphAdvances);
				p_allocator->transitionImageLayoutFront(fontImage,VK_IMAGE_LAYOUT_UNDEFINED, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL);
				p_allocator->copyBufferToImage(buffer, fontImage);
				p_allocator->transitionImageLayoutBack(fontImage, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, VK_IMAGE_LAYOUT_GENERAL );

				p_allocator->flush();
				memcpy_s(&fontMetadata->atlas, sizeof(fontAtlas), buffer.getData(), sizeof(fontAtlas));
				fontFile.close();
				return std::dynamic_pointer_cast<assets::asset>(createRef<assets::font>(fontImage,fontMetadata->glyphAdvances,fontMetadata->glyphScales,fontMetadata->glyphOffests));

			}
			else LN_CORE_ERROR("incorrect file format, expected .ttf!");
			fontFile.close();
			return nullptr;		
		}

   }
}

