#include "assetImporter.h"
#include <core/assets/importers/textureImporter.h>
#include <core/assets/importers/fontImporter.h>
namespace luna 
{
    namespace assets
    {
        using AssetImportFunction = std::function<ref<asset>(assetHandle, assetMetadata*)>;
        static std::map<assetType, AssetImportFunction> s_assetImportFunctions = {
            { assetType::texture, textureImporter::importTexture },
            { assetType::font   , fontImporter::importFont       },
        };
        ref<asset> assetImporter::importAsset(assetHandle handle,assetMetadata* metadata)
        {
            if (s_assetImportFunctions.find(metadata->assetType) == s_assetImportFunctions.end())
            {
                LN_CORE_ERROR("No importer available for asset type: {}", (uint16_t)metadata->assetType);
                return nullptr;
            }

            return s_assetImportFunctions.at(metadata->assetType)(handle, metadata);
        }
    }
}