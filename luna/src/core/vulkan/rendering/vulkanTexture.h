#pragma once
#include <core/rendering/texture.h>
namespace luna
{
	namespace vulkan
	{
		/**
		 * @brief base texture class.
		 * @note see specific platform implementation for explenation with functions
		 */
		class vulkanTexture : public renderer::texture
		{
		public:
			vulkanTexture(const std::string& filePath);
			vulkanTexture(const void* texelData, const uint32_t textureSize);
			vulkanTexture(const void* texelData, const glm::vec2& dimensions);
			virtual ~vulkanTexture() = default;

			virtual uint32_t getWidth() const override;
			virtual uint32_t getHeight() const override;

			virtual const std::string& getPath() const override;

			virtual void setData(void* data, uint32_t size)override;

			virtual void bind(uint32_t slot = 0) const override;

			virtual bool isLoaded() const override;

			virtual bool operator==(const texture& other) const override;
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

			virtual uint32_t getTileWidth();
			virtual uint32_t getTileWidth(const uint16_t& xIndex, const uint16_t& yIndex);

			virtual uint32_t getTileHeight();
			virtual uint32_t getTileHeight(const uint16_t& xIndex, const uint16_t& yIndex);

			virtual uint32_t addTile(const glm::vec2& dimensions);
			virtual uint32_t addTile(const uint32_t& width, const uint32_t& height);

			virtual glm::vec2 getTileDimensions(const glm::vec2& textureindex);
			virtual glm::vec2 getTextureUv(const glm::vec2& textureindex); // for texture atlasses;

		private:
			uint32_t tileWidth;
			uint32_t tileHeight;

			std::vector<std::vector<glm::vec2>> tileCustomTexCoords;
		};
	}
}

