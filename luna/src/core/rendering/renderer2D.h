#pragma once
#include <core/rendering/renderer.h>
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
			 */
			static void drawQuad(const glm::vec3& position, const glm::vec2& size);
			/**
			 * @brief draws a quad.
			 * 
			 * \param const glm::mat4& transform starting transformation.
			 */
			static void drawQuad(const glm::mat4& transform);
			/**
			 * @brief flushes all the draw commands to the base renderer.
			 * @see renderer::renderer
			 */
			static void flush();
		private:
		};
	}
}


