#pragma once
#include <core/assets/asset.h>
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