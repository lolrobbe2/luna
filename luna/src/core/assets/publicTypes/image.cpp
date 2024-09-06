#include <core/application.h>
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
#ifdef IMGUI_API

		void image::enableImgui(ref<image> image)
		{
			image->imGuiImageHandle = RENDERER->registerImGuiImage(image);
		}
		void image::enableImgui(std::vector<ref<image>> images)
		{
			for (ref<image> image : images)
				image::enableImgui(image);
		}
		void image::diableImgui()
		{
			RENDERER->unregisterImGuiImage(imGuiImageHandle);
		}
#endif // IMGUI_API
	}
}
