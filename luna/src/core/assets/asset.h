#pragma once
#include "lnpch.h"
#include <core/debug/uuid.h>
namespace luna 
{
	namespace assets 
	{
		using assetHandle = uuid;
		enum assetType : uint16_t
		{
			none = 0,
			texture,
			font,
			scene,
		};


		class asset
		{
		public:
			assetHandle assetHandle; //auto generate handle
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
		struct assetMetadata {
			assetHandle handle; //unique identifier of the asset
			char name[_MAX_FNAME]; //name of the asset
			char filePath[_MAX_PATH];
			uint64_t fileSizeBytes;
			assetType assetType;
		};

		struct textureAssetMetaData {
			assetMetadata baseMetaData; //standard metadata that each asset has.
			uint64_t width, height, channels;
			uint64_t imageByteSize;
		};
#pragma pack(pop)
	}
}