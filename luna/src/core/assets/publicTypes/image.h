#pragma once
#include <core/assets/asset.h>
#include <core/artemis/rendering/image.h>
namespace luna 
{
	namespace assets 
	{
		/**
		* @brief public asset type to allow outside interraction with assets.
		*/
		class LN_API image : public asset
		{
		public:
			virtual assets::assetType getType() const override;
		protected:
			friend class textureImporter;
			image(artemis::image& _image);
		private:
			artemis::image& _image = artemis::image();
		};
	}
}


