#pragma once
#include <core/vulkan/window/window.h>

namespace luna
{
	namespace renderer
	{
		/**
		 * @brief base texture class.
		 * @note see specific platform implementation for explenation with functions
		 */
		class texture
		{
		public:
			virtual ~texture() = default;

			virtual uint32_t getWidth() const = 0;
			virtual uint32_t getHeight() const = 0;

			virtual const std::string& getPath() const = 0;

			virtual void setData(void* data, uint32_t size) = 0;

			virtual void bind(uint32_t slot = 0) const = 0;

			virtual bool isLoaded() const = 0;

			virtual bool operator==(const texture& other) const = 0;
		};
		/**
		 * @brief texture2D api.
		 * @see texture
		 * @note see specific platform implementation for explenation with functions
		 */
		class texture2D : public texture
		{
		public:
			virtual ~texture2D() = default;
			ref<texture> create(const uint32_t& width, const uint32_t& height);
			ref<texture> create(const void* data, const uint32_t& size);
		};
		/**
		 * @brief textureAtlas api.
		 * @see texture
		 * @note see specific platform implementation for explenation with functions
		 */
		class textureAtlas : public texture
		{
		public:

			virtual ~textureAtlas() = default;

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
