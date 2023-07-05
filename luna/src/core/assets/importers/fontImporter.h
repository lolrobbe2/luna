#pragma once
#include <core/assets/asset.h>
#include <core/rendering/texture.h>
namespace luna 
{
	namespace assets
	{
		class fontImporter
		{
		public:
			static ref<asset> importFont(assetHandle handle, assetMetadata* metadata);
		};
	}
}


