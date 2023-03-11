#pragma once

#include <core/events/keyCodes.h>
#include <core/events/mouseCodes.h>

#include <glm/glm.hpp>

namespace luna 
{
	namespace input
	{
		class LN_API input
		{
		public:
			static bool isKeyPressed(keyCode key);
			static bool isMouseButtonPressed(mouseCode button);
			static glm::vec2 getMousePosition();
			static float getMouseX();
			static float getMouseY();
		};
	}
}