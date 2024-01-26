#include "image.h"
namespace luna 
{
	namespace assets 
	{
		assets::assetType image::getType() const
		{
			return assetType::texture;
		}
		image::image(artemis::image& _image)
		{
			this->_image = _image;
		}

	}
}
