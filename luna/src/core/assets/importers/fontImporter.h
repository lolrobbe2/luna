#pragma once
#ifndef _FONT_IMPORTER_
#define _FONT_IMPORTER_
#include <core/assets/asset.h>

namespace luna 
{
	namespace assets
	{
		class fontImporter
		{
		public:
			static ref<asset> importFont(assetHandle handle, assetMetadata* metadata);
		};
	}
}

#endif // !_FONT_IMPORTER_