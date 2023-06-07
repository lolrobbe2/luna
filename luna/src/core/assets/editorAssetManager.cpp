#include "editorAssetManager.h"
#include <core/assets/assetImporter.h>
namespace luna 
{
	namespace assets 
	{
		static utils::objectStorage<assetMetadata*> assetMetadataStorage;

		editorAssetManager::~editorAssetManager()
		{
			//assetMetadataStorage.clear(); TODO other branch
		}
		void editorAssetManager::loadImportedAssetsMetadata()
		{
			std::vector<std::filesystem::path> assetMetadataPaths;
			{
				for (auto& dir_entry : std::filesystem::recursive_directory_iterator("import"))
				{
					if (dir_entry.path().extension() == ".limp") {
						assetMetadataPaths.push_back(dir_entry.path());
					}
				}
			}
			assetMetadata tempMetadata;
			for(std::filesystem::path importPath : assetMetadataPaths)
			{
				std::ifstream importFile(importPath);
				importFile.read((char*)&tempMetadata, sizeof(assetMetadata));
				importFile.seekg(0);
				assetMetadata* assetMetadata = getMetadataPointer(tempMetadata.assetType);
				importFile.read((char*)assetMetadata, getMetadataStructSize(tempMetadata.assetType));
				assetMetadataStorage.putValue((utils::storageObject*)&assetMetadata->handle, assetMetadata);
			}
			
		}
		void editorAssetManager::loadAsset(assetHandle handle, const assetType type)
		{
			ref<asset> importedAsset = assetImporter::importAsset(handle,assetMetadataStorage[handle].second);
			utils::storageObject* key = (utils::storageObject*)&handle;
			if (importedAsset.get()) assetStorage.putValue(key, importedAsset);
		}
		bool editorAssetManager::isAssetHandleValid(assetHandle handle)
		{
			return assetMetadataStorage.hasValue(handle);
		}
		void editorAssetManager::importAsset(const std::string& filePath,const assetType type)
		{
			assetMetadata* metaData = getMetadataPointer(type);
			metaData->assetType = type;
			std::filesystem::path _filePath(filePath);

			std::string path = _filePath.parent_path().string();
			std::string filename = _filePath.filename().string();
			memcpy(metaData->filePath, path.c_str(), path.size());
			memcpy(metaData->name, filename.c_str(), filename.size());
			ref<asset> importedAsset = assetImporter::importAsset(metaData->handle, metaData);
			saveImportData(metaData);
		}
		assetMetadata* editorAssetManager::getMetadataPointer(const assetType type)
		{
			switch (type)
			{
			case luna::assets::none:
				LN_CORE_WARN("assetType::none is not valid");
				return nullptr;
			case luna::assets::texture:
				return (assetMetadata*) new textureAssetMetaData();
			case luna::assets::font:
				return nullptr;
			case luna::assets::scene:
				return nullptr;
			default:
				LN_CORE_WARN("assetType::default is not valid /assetType not found");
				return nullptr;
			}
		}
		size_t editorAssetManager::getMetadataStructSize(const assetType type)
		{
			switch (type)
			{
			case luna::assets::none:
				break;
			case luna::assets::texture:
				return sizeof(textureAssetMetaData);
			case luna::assets::font:
				break;
			case luna::assets::scene:
				break;
			default:
				break;
			}
			return size_t();
		}
		void editorAssetManager::saveImportData(assetMetadata* assetMetadata)
		{
			std::filesystem::path filePath;
			{
				std::string _filePath = "import/";
				_filePath += assetMetadata->name;
				filePath = _filePath;
			}
			filePath.replace_extension(".limp");
			std::ofstream importFile;
			mkdir("import");
			importFile.open((filePath.c_str()));
			importFile.write((char*)assetMetadata, getMetadataStructSize(assetMetadata->assetType));
			importFile.close();

			
			/*hide import folder very valuable*/
			int attr = GetFileAttributesA("import");
			if ((attr & FILE_ATTRIBUTE_HIDDEN) == 0) {
				SetFileAttributesA("import", attr | FILE_ATTRIBUTE_HIDDEN);
			}
			
		}
	}
}