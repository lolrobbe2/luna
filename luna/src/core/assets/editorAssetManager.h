#pragma once
#include <core/assets/assetManagerBase.h>
namespace luna 
{
	namespace assets 
	{
		class LN_API editorAssetManager : assetManagerBase
		{
		public:
			editorAssetManager() = default;
			virtual ~editorAssetManager();
			virtual void loadImportedAssetsMetadata() override;
			virtual void loadAsset(assetHandle handle, const assetType type) override;
			virtual bool isAssetHandleValid(assetHandle handle) override;
			virtual void importAsset(const std::string& filePath, const assetType type) override;
		private:
			static assetMetadata* getMetadataPointer(const assetType type);
			static size_t getMetadataStructSize(const assetType type);
			static void saveImportData(assetMetadata* assetMetadata);
		};
	}
}


