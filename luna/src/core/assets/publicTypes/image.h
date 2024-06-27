#ifndef _PUBLIC_IMAGE_
#define _PUBLIC_IMAGE_
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
			~image() override //by doing this we ensure that the deconstructor gets called when the object is destroyed even tho the parrent class has a deconstructor!
			{
				unbind();
			};
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
			_ALWAYS_INLINE_ glm::vec4 getUv() const { return _image; }
			_ALWAYS_INLINE_ operator glm::vec4() const { return _image; }
			_ALWAYS_INLINE_ std::array<glm::vec2, 4> getUvCoords() const { return _image; }
			_ALWAYS_INLINE_ operator std::array<glm::vec2, 4>() const { return _image; }
			_ALWAYS_INLINE_ VkImageLayout getCurrentLayout() const { return _image; }
			_ALWAYS_INLINE_ operator VkImageLayout() const { return _image; }
			_ALWAYS_INLINE_ bool isBound() const { return descriptorSetIndex == UINT32_MAX && imageIndex == UINT8_MAX; }
			_ALWAYS_INLINE_ operator bool() const { return isBound(); }  
			_ALWAYS_INLINE_ operator float() const { return imageIndex; }
			_ALWAYS_INLINE_ float getImageIndex() const { return imageIndex; }
#ifndef IMGUI_DISABLE
			static void enableImgui(ref<image> image);
			static void enableImgui(std::vector<ref<image>> images);
			void diableImgui();
			ImTextureID getGuiImageHandle() { return imGuiImageHandle; }
#endif
			void bind(uint32_t descriptorSetIndex,uint8_t imageIndex,std::vector<uint8_t>* p_freeImageIndices);
			void unbind();
			image(artemis::image& _image);
	
		private:
			uint32_t descriptorSetIndex = UINT32_MAX; //both descriptorSetIndex + imageIndex
			uint8_t imageIndex = UINT8_MAX;
			std::vector<uint8_t>* p_freeImageIndices;
#ifdef IMGUI_API
			ImTextureID imGuiImageHandle = 0;
#endif
		protected:
			artemis::image& _image = *new artemis::image();
		};
	}
}


#endif