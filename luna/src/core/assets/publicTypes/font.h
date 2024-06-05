#pragma once
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
			virtual assets::assetType getType() const override;
			const ref<assets::image> getGlyph(char hlyph) const;
			const glm::vec2& font::getAdvance(const char character) const;
		private:
			ref<assets::image> glyphs[256];
			glm::vec2* advances;
		};
	}
}


