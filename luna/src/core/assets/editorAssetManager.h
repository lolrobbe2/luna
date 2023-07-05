#pragma once
#include <core/assets/assetManagerBase.h>
namespace luna 
{
	namespace assets 
	{
		class LN_API editorAssetManager : public assetManagerBase
		{
		public:
			editorAssetManager() = default;
			virtual ~editorAssetManager();

			virtual void loadImportedAssetsMetadata() override;
			virtual ref<asset> getAsset(assetHandle handle) override;
			virtual ref<asset> getAsset(const std::string& name) override;
			virtual assetMetadata* getAssetMetadata(assetHandle handle) override;
			virtual assetMetadata* getAssetMetadata(const std::string& filename) override;
			virtual void loadAsset(assetHandle handle) override;
			virtual bool isAssetHandleValid(assetHandle handle) override;
			virtual bool isAssetHandleValid(const std::string& filename) override;
			virtual bool isAssetHandleLoaded(assetHandle handle) override;
			virtual assetHandle importAsset(const std::string& filePath, const assetType type) override;
		private:
			static assetMetadata* getMetadataPointer(const assetType type);
			static size_t getMetadataStructSize(const assetType type);
			static void saveImportData(assetMetadata* assetMetadata);
		};
	}
}


