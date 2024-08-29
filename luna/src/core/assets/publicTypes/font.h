#ifndef _PUBLIC_FONT_
#define _PUBLIC_FONT_
#include <core/assets/publicTypes/image.h>
#define GLM_VEC2_GLYPH_WIDTH static glm::vec2 glyphDimensions {GLYPH_WIDTH,GLYPH_HEIGHT}
namespace luna 
{
	namespace assets 
	{
		GLM_VEC2_GLYPH_WIDTH;
		class LN_API font : public image
		{
		public:

			font(artemis::image& _image,glm::vec2* advances,glm::vec2* scales);
			~font() { unbind(); }
			virtual assets::assetType getType() const override;
			const ref<assets::image> getGlyph(char hlyph) const;
			const glm::vec2& getAdvance(const char character) const
			{
				return advances[character];
			}
			const glm::vec2& getVirtualExtent(const char character) const
			{
				return glyphDimensions / glyphScales[character];
			}
			virtual void bind(uint32_t descriptorSetIndex, uint8_t imageIndex, std::vector<uint8_t>* p_freeImageIndices) override;
		private:
			ref<assets::image> glyphs[256];
			//soft refs to fontAssetMetadata (cleanded up by asset manager);
			glm::vec2* glyphScales = nullptr; 
			glm::vec2* advances = nullptr;
		};
	}
}


#endif