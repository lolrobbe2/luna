#pragma once
#include <nodes/canvasItem.h>
#ifndef LN_EVENT_COMPONENT
#define LN_EVENT_COMPONENT() addOrReplaceComponent<eventComponent>().guiEvent = [this](Event& event) { this->guiEvent(event); }
#endif
namespace luna 
{
	namespace nodes 
	{
		class LN_API controlNode : public canvasItem
		{
		public:
			LN_CLASS(controlNode, canvasItem);
			virtual void guiEvent(Event& event);
			float getOffset();
			//rect2 getRect()
			void grabClickFocus();
			void grabFocus();
			bool hasFocus();
			void releaseFocus();
			glm::vec2 getPosition();
			void setPosition(glm::vec2 position);
			glm::vec2 getScale();
			void setScale(glm::vec2 position);
		};
	}
}


