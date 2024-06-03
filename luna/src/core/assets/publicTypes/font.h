#pragma once
#include "image.h"
namespace luna 
{
	namespace assets 
	{
		
		class font : public image
		{
		public:

			font(artemis::image& _image);
			~font() { unbind(); }
			virtual assets::assetType getType() const override;
			const ref<assets::image> getGlyph(char hlyph) const;
			ref<assets::image> glyphs[256];
		};
	}
}


