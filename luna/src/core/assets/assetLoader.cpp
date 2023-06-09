#include "assetLoader.h"
namespace luna 
{
	namespace assets 
	{
        using assetLoadFunction = std::function<ref<asset>(assetHandle, assetMetadata*)>;
        static std::map<assetType, assetLoadFunction> s_assetLoadFunctions = {
            { assetType::texture, assetLoader::loadTexture },
            { assetType::font   , assetLoader::importFont       },
        };
        ref<asset> assetLoader::loadAsset(assetHandle handle, assetMetadata* metadata)
        {
            if (s_assetLoadFunctions.find(metadata->assetType) == s_assetLoadFunctions.end())
            {
                LN_CORE_ERROR("No importer available for asset type: {}", (uint16_t)metadata->assetType);
                return nullptr;
            }

            return s_assetLoadFunctions.at(metadata->assetType)(handle, metadata);
        }
	}
}