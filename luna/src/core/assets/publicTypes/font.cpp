#include "font.h"
#include <core/debug/debugMacros.h>
namespace luna
{
	namespace assets
	{
		
		font::font(artemis::image& _image,glm::vec2* advances,glm::vec2* scales,glm::vec2* offsets) : image(_image)
		{
			LN_UNROLL_LOOP
			for (size_t glyph = 0; glyph < 256; glyph++)
			{
				int yStart = glyph / 16;
				int xStart = glyph % 16;
				LN_ERR_FAIL_COND_MSG(!(xStart < 16 && yStart < 16), "character out of bounds!");
				glm::vec2 uvStart = { (float)xStart / 16,(float)yStart / 16 };
				glm::vec2 uvEnd = { (float)(xStart + 1) / 16,(float)(yStart + 1) / 16 };
				ref<image> glyphTexture = createRef<image>(_image.getSubImage({ GLYPH_WIDTH,GLYPH_HEIGHT }, { uvStart,uvEnd }));
				glyphs[glyph] = glyphTexture;
			}
		
			this->advances = advances;
			this->offsets = offsets;
			this->glyphScales = scales;
		}
		const ref<assets::image> font::getGlyph(char glyph) const
		{
			return glyphs[glyph];
		}

		void font::bind(uint32_t descriptorSetIndex, uint8_t imageIndex, std::vector<uint8_t>* p_freeImageIndices)
		{
			image::bind(descriptorSetIndex, imageIndex, p_freeImageIndices);
			for (ref<image> glyph : glyphs)
				glyph->bind(descriptorSetIndex, imageIndex, p_freeImageIndices);
			

		}

		assets::assetType font::getType() const
		{
			return assetType::FONT_ATLAS;
		}
	}
}