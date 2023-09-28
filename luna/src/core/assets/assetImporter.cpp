#include "assetImporter.h"
#include <core/assets/importers/textureImporter.h>
#include <core/assets/importers/fontImporter.h>
#include <core/debug/debugMacros.h>
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
            LN_ERR_FAIL_COND_V_MSG(s_assetImportFunctions.find(metadata->assetType) == s_assetImportFunctions.end(), nullptr, "No importer available for asset type!");
            return s_assetImportFunctions.at(metadata->assetType)(handle, metadata);
        }
    }
}