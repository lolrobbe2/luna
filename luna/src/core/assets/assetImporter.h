#pragma once
#include <core/assets/asset.h>
//inspired by the hazelengine asset importer
namespace luna 
{
	namespace assets {
		class assetImporter
		{
		public:
			static ref<asset> importAsset(assetHandle handle, assetMetadata* metadata);
		};
	}

}