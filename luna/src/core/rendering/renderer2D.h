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
			static void drawQuad(glm::vec2 size);
		private:
		};
	}
}


