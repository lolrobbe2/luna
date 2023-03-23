#pragma once
#include <core/rendering/texture.h>
namespace luna
{
	namespace vulkan
	{
		/**
		 * @brief texture implementation in vulkan.
		 */
		class vulkanTexture : public renderer::texture
		{
		public:
			vulkanTexture(const std::string& filePath);
			vulkanTexture(const uint64_t& handle, const glm::vec2& dimensions);
			virtual ~vulkanTexture();

			virtual uint32_t getWidth() const override;
			virtual uint32_t getHeight() const override;

			virtual const std::string& getPath() const override;

			virtual void setData(void* data, uint32_t size) override;

			virtual void bind(uint32_t slot = 0) const override;

			virtual bool isLoaded() const override;

		protected:
			VkBuffer buffer = VK_NULL_HANDLE;
			VkImage imageHandle = VK_NULL_HANDLE;
			VkImageView imageViewHandle = VK_NULL_HANDLE;
		};
		/**
		 * @brief texture2D api.
		 * @see texture
		 * @note see specific platform implementation for explenation with functions
		 */
		class vulkanTexture2D : public renderer::texture2D 
		{
		public:
			vulkanTexture2D(const std::string& filePath);
			vulkanTexture2D(const void* texelData, const uint32_t textureSize);
			vulkanTexture2D(const void* texelData, const glm::vec2& dimensions);
			~vulkanTexture2D() = default;
		};
		/**
		 * @brief textureAtlas api.
		 * @see texture
		 * @note see specific platform implementation for explenation with functions
		 */
		class vulkanTextureAtlas : public renderer::textureAtlas
		{
		public:
			vulkanTextureAtlas(const std::string& filePath, const glm::vec2& texDimensions, const glm::vec2& tileDimensions);
			vulkanTextureAtlas(const std::string& filePath, const glm::vec2& texDimensions);
			virtual ~vulkanTextureAtlas() = default;

			virtual uint32_t getTileWidth() override;
			virtual uint32_t getTileWidth(const uint16_t& xIndex, const uint16_t& yIndex) override;

			virtual uint32_t getTileHeight() override;
			virtual uint32_t getTileHeight(const uint16_t& xIndex, const uint16_t& yIndex) override;

			virtual glm::vec2 addTile(const glm::vec2& dimensions);
			virtual glm::vec2 addTile(const uint32_t& width, const uint32_t& height) override;

			virtual glm::vec2 getTileDimensions(const glm::vec2& textureindex) override;
			virtual glm::vec2 getTextureUv(const glm::vec2& textureindex) override; // for texture atlasses;
			ref<renderer::texture> getTileAsTexture(const glm::vec2& textureindex);
		private:

			struct freeStripeBlock
			{
				glm::vec2 start;
				glm::vec2 end;
				uint32_t getWidth() { return end.x - start.x; };
				uint32_t getHeight() { return end.y - start.y; };
			};
			struct stripe {
				std::vector<freeStripeBlock> stripeVector;
			};
			uint32_t tileWidths;
			uint32_t tileHeight;
			
			std::vector<std::vector<glm::vec4>> tileCustomTexCoords;
			std::vector<stripe> atlas;
			VkBuffer buffer = VK_NULL_HANDLE;
			VkImage imageHandle = VK_NULL_HANDLE;
			VkImageView imageViewHandle = VK_NULL_HANDLE;
		private:
			freeStripeBlock* checkStripe(stripe& stripe,const uint32_t& width, const uint32_t& height);
		};
		/**
		 * @brief font implementation in vulkan.
		 */
		class LN_API vulkanFont : public renderer::font
		{
		public:
			//16*300 (width) = 4800
			//14*300 (height) = 4200
			//32 dec - 127 decimal;
			vulkanFont(const std::string& filePath);
			virtual ~vulkanFont() = default;
			virtual ref<renderer::texture> getGlyph(char character) override;
			virtual glm::vec2 getAdvance(char character) override;
			virtual glm::vec2 getScale(char character) override;
		private:
			/**
			 * @brief allocates and creates the atlas texture from wich glyph can be sampled from.
			 *
			 */
			void createFontTexture();
			/**
			 * @brief creates a font glyph and gurantees the texture height and width to be 300 by 300 pixels.
			 *
			 * \param const stbtt_fontinfo* info
			 * \param int codePoint (char code to create glyph from)
			 * \param float* xscale: relative scale to get 1.0f width back. (300 / xscale)
			 * \param float* yscale: relative scale to get 1.0f height back. (300 / yscale)
			 * \param int* newXoff: relative xoffset.
			 * \param newYoff: relative yoffset.
			 * \return stbi_uc* pointer to texure data.
			 */
			stbi_uc* createGlyph(const stbtt_fontinfo* info, int codePoint, float* xscale, float* yscale, int* newXoff, int* newYoff);
			
			void writeGlyphsIntoBuffer();
		private:
			struct glyph
			{
				stbi_uc _glyph[300*300];
			};
			const static int width = 4800;
			const static int height = 4800;
			VkBuffer testBuffer = VK_NULL_HANDLE;
			std::vector<VkBuffer> buffer;
			VkImage imageHandle = VK_NULL_HANDLE;
			VkImageView imageViewHandle = VK_NULL_HANDLE;
		};
	
	}
}

