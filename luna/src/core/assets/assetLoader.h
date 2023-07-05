#pragma once
#include <core/assets/asset.h>
namespace luna 
{
	namespace assets 
	{
		class assetLoader
		{
		public:
			static ref<asset> loadAsset(assetHandle handle, assetMetadata* metadata);
		};
	}
}


