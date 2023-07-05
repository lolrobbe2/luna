#pragma once
#include <core/assets/asset.h>
#include <core/rendering/texture.h>
namespace luna
{
	namespace assets 
	{
		class textureImporter
		{
		public:
			static ref<asset> importTexture(assetHandle handle, assetMetadata* metadata);
		};
	}
}