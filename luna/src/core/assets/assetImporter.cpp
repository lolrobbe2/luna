#include "assetImporter.h"
#include <core/assets/importers/textureImporter.h>
namespace luna 
{
    namespace assets
    {
        using AssetImportFunction = std::function<ref<asset>(assetHandle, assetMetadata*)>;
        static std::map<assetType, AssetImportFunction> s_AssetImportFunctions = {
            { assetType::texture, textureImporter::importTexture }
        };
        ref<asset> assetImporter::importAsset(assetHandle handle,assetMetadata* metadata)
        {
            if (s_AssetImportFunctions.find(metadata->assetType) == s_AssetImportFunctions.end())
            {
                LN_CORE_ERROR("No importer available for asset type: {}", (uint16_t)metadata->assetType);
                return nullptr;
            }

            return s_AssetImportFunctions.at(metadata->assetType)(handle, metadata);
        }
    }
}