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
			ref<image> getGlyph(char hlyph);
			ref<image> glyphs[256];
		};
	}
}


