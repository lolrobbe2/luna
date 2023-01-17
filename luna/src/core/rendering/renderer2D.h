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
		class renderer2D
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
			static void BeginScene();
			/**
			 * @brief ends the 2D renderer scene.
			 */
			static void endScene();
			/**
			 * @brief draws a quad.
			 *
			 * \param const glm::vec3& position starting position
			 * \param const glm::vec2& size starting size;
			 * \param const glm::vec3& color rgb color value
			 */
			static void drawQuad(const glm::vec3& position, const glm::vec2& size, const ref<texture>& texture);
			/**
			 * @brief draws a quad.
			 *
			 * \param const glm::vec3& position starting position
			 * \param const glm::vec2& size starting size;
			 * \param const glm::vec3& color rgb color value
			 */
			static void drawQuad(const glm::vec3& position, const glm::vec2& size, const glm::vec4& color);
			/**
			 * @brief draws a quad.
			 * 
			 * \param const glm::vec3& position starting position
			 * \param const glm::vec2& size starting size;
			 */
			static void drawQuad(const glm::vec3& position, const glm::vec2& size);
			/**
			 * @brief draws a quad.
			 * 
			 * \param const glm::mat4& transform starting transformation.
			 */
			static void drawQuad(const glm::mat4& transform,const glm::vec4& color);
			/**
			 * @brief flushes all the draw commands to the base renderer.
			 * @see renderer::renderer
			 */
			static void flush();
			/**
			 * @brief returns renderer2D stats.
			 */
			static statistics getStats();

		private:
			inline static ref<texture> image;
		};
	}
}


