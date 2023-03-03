#pragma once
#include <core/vulkan/window/window.h>
#include <stb_truetype.h>

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

			inline void setDestroy(const bool& destroy) { this->destroy = destroy; };
			inline void setUv(const glm::vec2& _uvStart,const glm::vec2& _uvEnd) { uvStart = _uvStart; uvEnd = _uvEnd; };
			std::vector<glm::vec2> getUv() { return{ { uvStart.x, uvStart.y }, { uvEnd.x, uvStart.y }, { uvEnd.x, uvEnd.y }, { uvStart.x, uvEnd.y } }; };
			static ref<texture> create(const std::string& filePath);
			static ref<texture> create(const uint64_t& handle,const glm::vec2& dimensions);
			inline uint64_t handle() { return _handle; };
		protected:
			bool destroy = true;
			uint64_t _handle;
			void* data;
			uint32_t width;
			uint32_t height;
			glm::vec2 uvStart = {0.0f,0.0f};
			glm::vec2 uvEnd = { 1.0f,1.0f };

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

		class LN_API font 
		{
		public:
			//16*300 (width) = 4800
			//14*300 (height) = 4200
			//32 dec - 127 decimal;
			virtual ~font() = default;
			virtual ref<texture> getGlyph(char character) = 0;
			virtual glm::vec2 getAdvance(char character) = 0;
			virtual glm::vec2 getScale(char charcater) = 0;
			uint64_t handle() { return _handle; };
			static ref<font> create(const std::string& filePath);
		protected:
			const static char startIndex = 0;
			stbtt_fontinfo fontInfo;
			uint64_t _handle;
			void* data;
			std::vector<glm::vec2> glypScales;
			std::vector<glm::vec2> glypAdvances;
		private:

		};
	}
}
