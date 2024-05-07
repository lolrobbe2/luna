#include "assetLoader.h"
#include <core/assets/loaders/textureLoader.h>
#include <core/assets/loaders/fontLoader.h>
namespace luna 
{
	namespace assets 
	{
        static ref<artemis::allocator> p_allocator;
        using assetLoadFunction = std::function<ref<asset>(assetHandle, assetMetadata*)>;
        static std::map<assetType, assetLoadFunction> s_assetLoadFunctions = {
            { assetType::texture, textureLoader::loadTexture },
            { assetType::font   , fontLoader::loadFont  },
        };
        void assetLoader::setAllocator(const ref<artemis::allocator> p_allocator)
        {
            assets::p_allocator = p_allocator;
        }
        ref<asset> assetLoader::loadAsset(assetHandle handle, assetMetadata* metadata)
        {
            if (s_assetLoadFunctions.find(metadata->assetType) == s_assetLoadFunctions.end())
            {
                LN_CORE_ERROR("No loader available for asset type: {}", (uint16_t)metadata->assetType);
                return nullptr;
            }

            return s_assetLoadFunctions.at(metadata->assetType)(handle, metadata);
        }
	}
}