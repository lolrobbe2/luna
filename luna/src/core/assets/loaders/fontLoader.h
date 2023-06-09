#pragma once
#include <core/assets/asset.h>
namespace luna 
{
	namespace assets
	{
		class fontLoader
		{
		public:
			static ref<asset> loadFont(assetHandle handle, assetMetadata* metadata);
		};
	}
}


