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
		class LN_API texture
		{
		public:
			virtual ~texture() = default;

			virtual uint32_t getWidth() const = 0;
			virtual uint32_t getHeight() const = 0;

			virtual const std::string& getPath() const = 0;

			virtual void setData(void* data, uint32_t size) = 0;

			virtual void bind(uint32_t slot = 0) const = 0;

			virtual bool isLoaded() const = 0;

			//virtual bool operator==(const texture& other) const = 0;

			static ref<texture> create(const std::string& filePath);
			inline uint64_t handle() { return _handle; };
		protected:
			uint64_t _handle;
			void* data;
			uint32_t width;
			uint32_t height;
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
			static ref<texture> create(const std::string& filePath);
			static ref<texture> create(const uint32_t& width, const uint32_t& height);
			static ref<texture> create(const void* data, const uint32_t& size);
		};
		/**
		 * @brief textureAtlas api.
		 * @see texture
		 * @note see specific platform implementation for explenation with functions
		 */
		class LN_API textureAtlas : public texture
		{
		public:

			virtual ~textureAtlas() = default;

			virtual uint32_t getTileWidth() = 0;
			virtual uint32_t getTileWidth(const uint16_t& xIndex, const uint16_t& yIndex)  = 0;

			virtual uint32_t getTileHeight() = 0;
			virtual uint32_t getTileHeight(const uint16_t& xIndex, const uint16_t& yIndex) = 0;

			virtual glm::vec2 addTile(const glm::vec2& dimensions) = 0;
			virtual glm::vec2 addTile(const uint32_t& width, const uint32_t& height) = 0;

			virtual glm::vec2 getTileDimensions(const glm::vec2& textureindex) = 0;
			virtual glm::vec2 getTextureUv(const glm::vec2& textureindex) = 0; // for texture atlasses;

		private:
			uint32_t tileWidth;
			uint32_t tileHeight;

			std::vector<std::vector<glm::vec2>> tileCustomTexCoords;
		};
	}
}
