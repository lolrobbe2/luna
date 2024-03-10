#pragma once
#include <core/assets/assetManagerBase.h>
namespace luna
{
	namespace assets
	{
		class assetManager
		{
		public:
			static void init(bool editor);
			/**
			* simular to std::filesystem::currentPath() but this function does not override the std::filesystem::currentPath.
			*/
			static void setImportDirectory(const std::filesystem::path& directory);
			/**
			* @brief returns an important asset and loads it if needed.
			*/
			template<typename T>
			static ref<T> getAsset(const assetHandle handle)
			{
				if (!isAssetHandleValid(handle))
				{
					LN_CORE_ERROR("asset has not been imported! \n handle = {0}", ((uuid)handle));
					return ref<T>();
				}
				if (!assetManagerRef->isAssetHandleLoaded(handle))
					loadAsset(handle);

				return ref<T>(assetManagerRef->getAsset(handle).get());
			}
			/**
			 * @brief identical to getAsset with handle but slower.
			 */
			template<typename T>
			static ref<T> getAsset(const std::string& name)
			{
				return std::dynamic_pointer_cast<T>(assetManagerRef->getAsset(name));
			}
			static assetMetadata* getAssetMetadata(const assetHandle handle);
			static assetMetadata* getAssetMetadata(const std::string& filename);
			/**
			 * @brief checks the metaDataStorage if an asset has been imported.
			 */
			static bool isAssetHandleValid(assetHandle handle);
			/**
			 * @brief checks the metaDataStorage if an asset has been imported.
			 */
			static bool isAssetHandleValid(const std::string& filename);
			/**
			 * @brief checks the assetStorage if the asset has been loaded.
			 */
			static bool isAssetHandleLoaded(assetHandle handle);
			/**
			 * @brief imports an asset that has not been registered/ has a .limp file
			 */
			static assetHandle importAsset(const std::string& filePath, const assetType type);
			static void loadImportedAssetMetaData();

		private:
			static void loadAsset(assetHandle handle);
			inline static ref<assetManagerBase> assetManagerRef;
		};
	}
}

