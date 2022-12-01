#pragma once
#include <core/vulkan/window/window.h>

namespace luna
{
	namespace renderer
	{
		/**
		 * @brief base textuer class.
		 */
		class texture
		{
			virtual ~texture() = default;

			virtual uint32_t getWidth() const = 0;
			virtual uint32_t getHeight() const = 0;

			virtual glm::vec2 gettextureUv(); // for texture atlasses;
			virtual const std::string& getPath() const = 0;

			virtual void setData(void* data, uint32_t size) = 0;

			virtual void bind(uint32_t slot = 0) const = 0;

			virtual bool isLoaded() const = 0;

			virtual bool operator==(const texture& other) const = 0;
		};
		class texture2D : public texture
		{
			ref<texture> create(const uint32_t& width, const uint32_t& height);
			ref<texture> create(const void* data, const uint32_t& size);
		};
		class textureAtlas : public texture
		{
		public:
			virtual uint32_t getTileWidth();
			virtual uint32_t getTileWidth(const uint16_t xIndex);

			virtual uint32_t getTileHeight();
			virtual uint32_t getTileHeight(const uint16_t yIndex);
		private:
			uint32_t tileWidth;
			uint32_t tileHeight;

			std::vector<std::vector<uint32_t>> tileCustomUv;
		};
	}
}
