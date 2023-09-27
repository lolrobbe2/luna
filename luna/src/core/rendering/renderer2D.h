#pragma once
#include <core/rendering/renderer.h>
#include <core/rendering/texture.h>
namespace luna
{
	namespace renderer
	{
		/**
		 * @brief 2D renderer api  runs onTop of the renderer api.
		 * @see renderer::renderer
		 */
		class LN_API renderer2D
		{
		public:
			/**
			 * @brief struct containing renderer2D statistics.
			 */
			struct statistics
			{
				uint32_t drawCalls = 0;
				uint32_t quadCount = 0;

				uint32_t getTotalVertexCount() const { return quadCount * 4; }
				uint32_t getTotalIndexCount() const { return quadCount * 6; }
			};
			/**
			 * @brief initializes the 2D renderer.
			 * 
			 */
			static void init();
			/**
			 * @brief shutsdown the 2D renderer and destroys the appropriate resources.
			 */
			static void shutdown();
			/**
			 * @brief starts a 2D renderer scene.
			 */
			_ALWAYS_INLINE_ static void BeginScene();
			/**
			 * @brief ends the 2D renderer scene.
			 */
			_ALWAYS_INLINE_ static void endScene();
			/**
			 * @brief .
			 * 
			 * \param position
			 * \param size
			 * \param font
			 * \param labelText
			 */
			_ALWAYS_INLINE_ static bool drawLabel(const glm::vec3& position, const glm::vec2& size, const ref<font>& font,const std::string labelText, const glm::vec4& bounds, uint8_t& startOutBounds);
			
			/**
			 * @brief draws a texture quad.
			 *
			 * \param const glm::vec3& position starting position
			 * \param const glm::vec2& size starting size;
			 * \param const glm::vec3& color rgb color value
			*/
			_ALWAYS_INLINE_ static bool drawCharQuad(const glm::vec3& position, const glm::vec2& size, const ref<texture>& texture,uint64_t handleIndex, glm::vec4 color = { 1.0f,1.0f,1.0f,1.0f }, glm::vec4 bounds = { -1.0f,1.0f,-1.0f,1.0f });
			/**
			 * @brief draws a texture quad.
			 *
			 * \param const glm::vec3& position starting position
			 * \param const glm::vec2& size starting size;
			 * \param const glm::vec3& color rgb color value
			 */
			_ALWAYS_INLINE_ static bool drawQuad(const glm::vec3& position, const glm::vec2& size, const ref<texture>& texture);
			/**
			 * @brief draws a texture quad.
			 * 
			 * \param const glm::mat4 transform
			 * \param const glm::vec4 color
			 * \param const ref<texture>& texture
			 */
			_ALWAYS_INLINE_ static bool drawQuad(const glm::mat4 transform,const glm::vec4 color, const ref<texture>& texture);
			/**
			 * @brief draws a quad.
			 *
			 * \param const glm::vec3& position starting position
			 * \param const glm::vec2& size starting size;
			 * \param const glm::vec3& color rgb color value
			 */
			_ALWAYS_INLINE_ static bool drawQuad(const glm::vec3& position, const glm::vec2& size, const glm::vec4& color);
			/**
			 * @brief draws a quad.
			 * 
			 * \param const glm::vec3& position starting position
			 * \param const glm::vec2& size starting size;
			 */
			_ALWAYS_INLINE_ static bool drawQuad(const glm::vec3& position, const glm::vec2& size);
			/**
			 * @brief draws a quad.
			 * 
			 * \param const glm::mat4& transform starting transformation.
			 */
			_ALWAYS_INLINE_ static bool drawQuad(const glm::mat4& transform,const glm::vec4& color);
			/**
			 * @brief flushes all the draw commands to the base renderer.
			 * @see renderer::renderer
			 */
			_ALWAYS_INLINE_ static void flush();
			/**
			 * @brief returns renderer2D stats.
			 */
			static statistics getStats();
			_ALWAYS_INLINE_ static uint64_t checkHandle(const uint64_t handle);
		private:
			_ALWAYS_INLINE_ static uint64_t textureInBatch(const uint64_t& handle);
			inline static ref<texture> blankImage;
		};
	}
}


