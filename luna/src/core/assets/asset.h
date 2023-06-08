#pragma once
#include "lnpch.h"
#include <core/debug/uuid.h>

#pragma region fonts
#define GLYPH_START_INDEX 0
#define FONT_ATLAS_WIDTH 1600
#define FONT_ATLAS_HEIGHT 1600

#define GLYPH_WIDTH 100
#define GLYPH_HEIGHT 100

#define FONT_ATLAS_ROWS (FONT_ATLAS_WIDTH / GLYPH_WIDTH)
#define FONT_ATLAS_COLUMNS (FONT_ATLAS_HEIGHT / GLYPH_HEIGHT)
#define FONT_ATLAS_GLYPH_AMOUNT (FONT_ATLAS_ROWS * FONT_ATLAS_COLUMNS)

#define GET_FONT_ATLAS_OFFSET_ROWS(xindex) (GLYPH_WIDTH * xindex)
#define GET_FONT_ATLAS_OFFSET_COLUMNS(yindex) (GLYPH_HEIGHT * yindex)

#define GET_FONT_BUFFER_OFFSET(xindex,yindex) (GET_FONT_ATLAS_OFFSET_ROWS(xindex) * GET_FONT_ATLAS_OFFSET_COLUMNS(yindex))
#pragma endregion

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

		struct textureAssetMetadata {
			assetMetadata baseMetaData; //standard metadata that each asset has.
			uint64_t width, height, channels;
			uint64_t imageByteSize;
		};

		struct fontAtlas
		{
			stbi_uc fontImage[FONT_ATLAS_WIDTH * FONT_ATLAS_WIDTH];
		};
		//new image height = 240
		struct fontAssetMetadata {
			assetMetadata baseMetaData;
			uint64_t width, height;
			fontAtlas atlas;
			glm::vec2 glyphScales[FONT_ATLAS_GLYPH_AMOUNT];
			glm::vec2 glyphAdvances[FONT_ATLAS_GLYPH_AMOUNT];
		};
#pragma pack(pop)
	}
}