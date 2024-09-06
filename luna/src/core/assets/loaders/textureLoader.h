#pragma once
#ifndef _TEXTURE_LOADER_
#define _TEXTURE_LOADER_
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
#endif // !_TEXTURE_LOADER_


