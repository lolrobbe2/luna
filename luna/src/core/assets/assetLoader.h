#pragma once
#include <core/assets/asset.h>
namespace luna 
{
	namespace artemis
	{
		class allocator; //forward declare
	}
	namespace assets 
	{
		
		class assetLoader
		{
		public:
			static void setAllocator(const ref<artemis::allocator> p_allocator);
			static ref<artemis::allocator> getAllocator();
			static ref<asset> loadAsset(assetHandle handle, assetMetadata* metadata);
		};
	}
}


