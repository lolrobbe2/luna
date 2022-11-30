#pragma once
#include <core/rendering/renderer.h>
namespace luna
{
	namespace renderer
	{
		class renderer2D
		{
		public:
			static void init();
			static void shutdown();
			static void BeginScene();
			static void endScene();
			static void drawQuad(const glm::vec3& position, const glm::vec2& size);
			static void drawQuad(const glm::mat4& transform);
			static void flush();
		private:
		};
	}
}


