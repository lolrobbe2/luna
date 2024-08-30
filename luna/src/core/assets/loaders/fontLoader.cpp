#include "fontLoader.h"
#include <core/assets/assetLoader.h>
#include <core/artemis/device/allocator.h>
#include <core/assets/publicTypes/font.h>
namespace luna
{
	namespace assets
	{



		ref<asset> fontLoader::loadFont(assetHandle handle, assetMetadata* metadata)
		{
			LN_PROFILE_FUNCTION();
			fontAssetMetadata* fontMetadata = (fontAssetMetadata*)metadata;
			
			ref<artemis::allocator> p_allocator = assetLoader::getAllocator();
			artemis::image& image = p_allocator->allocateImage({ fontMetadata->width, fontMetadata->height }, 1, VK_IMAGE_USAGE_SAMPLED_BIT | VK_IMAGE_USAGE_TRANSFER_DST_BIT);
			artemis::buffer& buffer = p_allocator->allocateBuffer(fontMetadata->width * fontMetadata->height, artemis::CPU_COPY, VK_BUFFER_USAGE_STORAGE_TEXEL_BUFFER_BIT | VK_BUFFER_USAGE_TRANSFER_SRC_BIT);

			buffer.setData(&fontMetadata->atlas,sizeof(fontAtlas));
			return std::dynamic_pointer_cast<assets::asset>(createRef<assets::font>(image,fontMetadata->glyphAdvances, fontMetadata->glyphScales,fontMetadata->glyphOffests));

		}
	}
}