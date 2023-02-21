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
			vulkanTexture(void* texelData, const glm::vec2& dimensions);
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
	}
}

