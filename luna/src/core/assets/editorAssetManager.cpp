#include "editorAssetManager.h"
#include <core/assets/assetImporter.h>
namespace luna 
{
	namespace assets 
	{
		static utils::objectStorage<assetMetadata*> assetMetadataStorage;
		static std::map<std::string, assetHandle> translationMap;
		editorAssetManager::~editorAssetManager()
		{
			//assetMetadataStorage.clear(); TODO other branch
		}
		void editorAssetManager::loadImportedAssetsMetadata()
		{
			std::vector<std::filesystem::path> assetMetadataPaths;
			{
				if (!std::filesystem::exists("import")) return;
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
				translationMap.insert({ assetMetadata->name,assetMetadata->handle });
			}
			
		}
		ref<asset> editorAssetManager::getAsset(assetHandle handle)
		{
			if (!isAssetHandleValid(handle))
			{
				LN_CORE_ERROR("asset has not been imported! \n handle = {0}", ((uuid)handle));
				return ref<asset>();
			}
			if (!isAssetHandleLoaded(handle))
				loadAsset(handle);
			return assetStorage.getValue(handle.getId()).second;
		}
		ref<asset> editorAssetManager::getAsset(const std::string& name)
		{
			auto result = translationMap.find(name);
			if(result != translationMap.end()) return getAsset(result->second);
			return ref<asset>();
		}
		assetMetadata* editorAssetManager::getAssetMetadata(assetHandle handle)
		{
			return  assetMetadataStorage.getValue(handle.getId()).second;;
		}
		assetMetadata* editorAssetManager::getAssetMetadata(const std::string& filename)
		{
			return getAssetMetadata(translationMap[filename]);
		}
		void editorAssetManager::loadAsset(assetHandle handle)
		{
			ref<asset> importedAsset = assetImporter::importAsset(handle,assetMetadataStorage[handle]);
			utils::storageObject* key = (utils::storageObject*)&handle;
			if (importedAsset.get()) assetStorage.putValue(key, importedAsset);
		}
		bool editorAssetManager::isAssetHandleValid(assetHandle handle)
		{
			return assetMetadataStorage.hasValue(handle);
		}

		bool editorAssetManager::isAssetHandleValid(const std::string& filename)
		{
			return translationMap.find(filename) != translationMap.end();
		}

		bool editorAssetManager::isAssetHandleLoaded(assetHandle handle)
		{
			return assetStorage.hasValue(handle);
		}

		assetHandle editorAssetManager::importAsset(const std::string& filePath,const assetType type)
		{
			assetMetadata* metaData = getMetadataPointer(type);
			metaData->assetType = type;
			std::filesystem::path _filePath(filePath);

			std::string path = _filePath.parent_path().string();
			std::string filename = _filePath.filename().string();
			if (translationMap.find(filename) != translationMap.end()) return translationMap[filename];
			memcpy(metaData->filePath, path.c_str(), path.size());
			memcpy(metaData->name, filename.c_str(), filename.size());
			ref<asset> importedAsset = assetImporter::importAsset(metaData->handle, metaData);
			if (importedAsset.get()) { 
				importedAsset->assetHandle = metaData->handle;
				saveImportData(metaData); 
				assetMetadataStorage.putValue((utils::storageObject*)&metaData->handle, metaData);
				translationMap.insert({ metaData->name,metaData->handle });
				assetStorage.putValue((utils::storageObject*)&metaData->handle, importedAsset);
				return metaData->handle;
			}
			return 0; //invalid, as in importing failed!
			
		}
		assetMetadata* editorAssetManager::getMetadataPointer(const assetType type)
		{
			switch (type)
			{
			case luna::assets::none:
				LN_CORE_WARN("assetType::none is not valid");
				return nullptr;
			case luna::assets::texture:
				return (assetMetadata*) new textureAssetMetadata();
			case luna::assets::font:
				return (assetMetadata*) new fontAssetMetadata();
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
				return sizeof(textureAssetMetadata);
			case luna::assets::font:
				return sizeof(fontAssetMetadata);
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