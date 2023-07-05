#include "assetManager.h"
#include <core/assets/editorAssetManager.h>
namespace luna
{
	namespace assets
	{

		void assetManager::init(bool editor)
		{

			if (editor) assetManagerRef = ref<assetManagerBase>(new editorAssetManager());
			//else LN_CORE_ERROR("runtime asset manager not implemented!");

			assetManagerRef->loadImportedAssetsMetadata();
		}
		void assetManager::loadAsset(assetHandle handle)
		{
			assetManagerRef->loadAsset(handle);
		}
		ref<asset> assetManager::getAsset(const assetHandle handle)
		{
			if (!isAssetHandleValid(handle)) 
			{
				LN_CORE_ERROR("asset has not been imported! \n handle = {0}",((uuid)handle)); 
				return ref<asset>();
			}
			if (!assetManagerRef->isAssetHandleLoaded(handle)) 
				loadAsset(handle);
			
			return assetManagerRef->getAsset(handle);
		}
		ref<asset> assetManager::getAsset(const std::string& name)
		{
			return assetManagerRef->getAsset(name);
		}

		assetMetadata* assetManager::getAssetMetadata(const std::string& filename)
		{
			return assetManagerRef->getAssetMetadata(filename);
		}

		assetMetadata* assetManager::getAssetMetadata(const assetHandle handle)
		{
			return assetManagerRef->getAssetMetadata(handle);
		}

		bool assetManager::isAssetHandleValid(assetHandle handle)
		{
			return assetManagerRef->isAssetHandleValid(handle);
		}
		bool assetManager::isAssetHandleValid(const std::string& filename)
		{
			return assetManagerRef->isAssetHandleValid(filename);
		}
		bool assetManager::isAssetHandleLoaded(assetHandle handle)
		{
			return assetManagerRef->isAssetHandleLoaded(handle);
		}
		assetHandle assetManager::importAsset(const std::string& filePath, const assetType type)
		{
			return assetManagerRef->importAsset(filePath,type);
		}
	}
}
