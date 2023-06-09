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
		public:
			virtual ref<asset> getAsset(assetHandle handle) = 0;
			virtual ref<asset> getAsset(const std::string& name) = 0;

			virtual void loadImportedAssetsMetadata() = 0;
			/**
			* @brief loads a valid assetHandle from the imported assets. (doesn not load .limp file this is allready loaded)
			*/
			virtual void loadAsset(assetHandle handle) = 0;
			/**
			* @brief checks the metaDataStorage if an asset has been imported.
			*/
			virtual bool isAssetHandleValid(assetHandle handle) = 0;
			/**
			* @brief checks the assetStorage if the asset has been loaded.
			*/
			virtual bool isAssetHandleLoaded(assetHandle handle) = 0;
			/**
			* @brief imports an asset that has not been registered/ has a .limp file
			*/
			virtual assetHandle importAsset(const std::string& filePath, const assetType type) = 0;
		protected:
			utils::objectStorage<ref<asset>> assetStorage;
		private:
			
		};

	}
}