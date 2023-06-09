#pragma once
#include <core/assets/asset.h>
namespace luna 
{
	namespace assets 
	{
		class textureLoader
		{
		public:
			static ref<asset> loadTexture(assetHandle handle, assetMetadata* metadata);
		};
	}
}


