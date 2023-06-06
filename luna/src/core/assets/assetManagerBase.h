#pragma once
#include <core/assets/asset.h>
#include <core/utils/objectStorage.h>
namespace luna
{
	namespace assets
	{
		/**
		* base interface for the assetManger 
		*/
		class assetManagerBase
		{
			static utils::objectStorage<ref<asset>> assetStorage;
		public:
			virtual void loadImportedAssetsMetadata() = 0;
			virtual void loadAsset(const std::string assetName,const assetType type) = 0;
			virtual bool isAssetHandleValid(assetHandle handle) = 0;
		private:
			
		};

	}
}