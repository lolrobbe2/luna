#ifndef _PUBLIC_FONT_
#define _PUBLIC_FONT_
#include "image.h"
namespace luna 
{
	namespace assets 
	{
		
		class font : public image
		{
		public:

			font(artemis::image& _image,glm::vec2* advances);
			~font() { unbind(); }
			LN_API virtual assets::assetType getType() const override;
			const ref<assets::image> getGlyph(char hlyph) const;
			const glm::vec2& font::getAdvance(const char character) const;
		private:
			ref<assets::image> glyphs[256];
			glm::vec2* advances = nullptr;
		};
	}
}


#endif