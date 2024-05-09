#pragma once
#include <core/assets/asset.h>

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


