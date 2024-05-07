#pragma once
#include <core/assets/asset.h>
namespace luna 
{
	namespace assets 
	{
		namespace artemis
		{
			class allocator; //forward declare
		}
		class assetLoader
		{
		public:
			static void setAllocator(const ref<artemis::allocator> p_allocator);
			static ref<asset> loadAsset(assetHandle handle, assetMetadata* metadata);
		};
	}
}


