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
			_ALWAYS_INLINE_ operator VkImage() const { return _image; }
			_ALWAYS_INLINE_ VkImage getImage() const { return _image; }
			_ALWAYS_INLINE_ operator VkImageView() const { return _image; }
			_ALWAYS_INLINE_ VkImageView getView() const { return _image; }
			_ALWAYS_INLINE_ operator VkFormat() const { return _image; }
			_ALWAYS_INLINE_ VkFormat getFormat() const { return _image; }
			_ALWAYS_INLINE_ operator glm::vec2() const { return _image; }
			_ALWAYS_INLINE_ glm::vec2 getExtent() const { return _image; }
			_ALWAYS_INLINE_ operator VkExtent3D() const { return _image; }
			_ALWAYS_INLINE_ bool isBound() const { return descriptorSetIndex == UINT32_MAX && imageIndex == UINT8_MAX; }
			_ALWAYS_INLINE_ operator bool() const { return isBound(); }
			void bind(uint32_t descriptorSetIndex,uint8_t imageIndex);
			void unbind();
			image(artemis::image& _image);
		private:
			artemis::image& _image = artemis::image();
			uint32_t descriptorSetIndex = UINT32_MAX; //both descriptorSetIndex + imageIndex
			uint8_t imageIndex = UINT8_MAX;
		};
	}
}


