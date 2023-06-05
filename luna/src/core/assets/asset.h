#pragma once
#include "lnpch.h"
#include <core/debug/uuid.h>
namespace luna 
{
	namespace assets 
	{
		using assetHandle = uuid;
		using assetType = uint64_t;
		enum assetType
		{
			none,
			texture,
			font,
			scene,
		};

		class asset
		{
		public:
			assetHandle handle; //auto generate handle
			virtual assetType getType() const = 0;
		private:

		};

		/**
		* structs are used because they are directly mappable from binary by using pointer.
		* this way the .limp (luna import) file can be loaded and de pointer aquired.
		* values can then be accesed like this: (assetMetaData*)data->handle;
		* 
		* @note this does mean that files need to be reimported when changes are made to the structs as they will no longer be valid.
		* this is because the values will be at an other offset.
		*/
		

//disable padding to not have extra bytes.
#pragma pack(push, 1)
		/**
		* @brief basic asset data, this info is dirrectly available from the assetManager without casting.
		*/
		struct assetMetaData {
			assetHandle handle; //unique identifier of the asset
			unsigned char name[_MAX_FNAME]; //name of the asset
			unsigned char filePath[_MAX_PATH];
			uint64_t fileSizeBytes;
			assetType assetType;
		};

		struct textureAssetMetaData {
			assetMetaData baseMetaData; //standard metadata that each asset has.

		};
#pragma pack(pop)
	}
}