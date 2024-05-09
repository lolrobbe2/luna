#include "font.h"
#include <core/debug/debugMacros.h>
namespace luna
{
	namespace assets
	{

		font::font(artemis::image& _image) : image(_image)
		{
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
		
		}
		ref<image> font::getGlyph(char glyph)
		{
			return glyphs[glyph];
		}
		assets::assetType image::getType() const
		{
			return assetType::fontAtlas;
		}
	}
}