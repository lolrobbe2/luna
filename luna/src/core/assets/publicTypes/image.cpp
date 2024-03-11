#include "image.h"
namespace luna 
{
	namespace assets 
	{
		assets::assetType image::getType() const
		{
			return assetType::texture;
		}
		void image::bind(uint32_t descriptorSetIndex, uint8_t imageIndex)
		{
			this->descriptorSetIndex = descriptorSetIndex;
			this->imageIndex = imageIndex;
		}
		void image::unbind()
		{
			this->descriptorSetIndex = UINT32_MAX;
			this->imageIndex = UINT8_MAX;
		}
		image::image(artemis::image& _image)
		{
			this->_image = _image;
		}

	}
}
