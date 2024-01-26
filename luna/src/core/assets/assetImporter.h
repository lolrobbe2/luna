#pragma once
#include <core/assets/asset.h>
//inspired by the hazelengine asset importer
namespace luna 
{
	namespace artemis
	{
		class allocator; //forward declare
	}
	namespace assets 
	{
		class assetImporter
		{
		public:
			static void setAllocator(const ref<artemis::allocator> p_allocator);
			static const ref<artemis::allocator> getAllocator();
			static ref<asset> importAsset(assetHandle handle, assetMetadata* metadata);
		};
	}

}