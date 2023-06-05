#pragma once
#include "lnpch.h"
#include <core/debug/uuid.h>
namespace luna
{
	namespace assets
	{
		using assetHandle = uuid;
		class assetManager
		{
			enum class assetType
			{
				none,
				texture,
				font,
				scene,
			};
		public:
			assetManager() = default;
			virtual ~assetManager() = default;

			
			virtual void loadAsset(const std::string assetName,const assetType type) = 0;
			virtual bool isAssetHandleValid
		private:
			
		};

	}
}