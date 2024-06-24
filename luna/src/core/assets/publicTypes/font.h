#ifndef _PUBLIC_FONT_
#define _PUBLIC_FONT_
#include "image.h"
namespace luna 
{
	namespace assets 
	{
		
		class LN_API font : public image
		{
		public:

			font(artemis::image& _image,glm::vec2* advances);
			~font() { unbind(); }
			virtual assets::assetType getType() const override;
			const ref<assets::image> getGlyph(char hlyph) const;
			const glm::vec2& getAdvance(const char character) const
			{
				return advances[character];
			}
		private:
			ref<assets::image> glyphs[256];
			glm::vec2* advances = nullptr;
		};
	}
}


#endif