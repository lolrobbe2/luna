#include "image.h"
namespace luna 
{
	namespace assets 
	{
		assets::assetType image::getType() const
		{
			return assetType::TEXTURE;
		}
		void image::bind(uint32_t descriptorSetIndex, uint8_t imageIndex, std::vector<uint8_t>* p_freeImageIndices)
		{
			this->descriptorSetIndex = descriptorSetIndex;
			this->imageIndex = imageIndex;
			this->p_freeImageIndices = p_freeImageIndices;
		}
		void image::unbind()
		{
			if(imageIndex != UINT8_MAX) p_freeImageIndices->push_back(imageIndex);
			this->descriptorSetIndex = UINT32_MAX;
			this->imageIndex = UINT8_MAX;
		}
		image::image(artemis::image& _image)
		{
			this->_image = _image;
		}

	}
}
