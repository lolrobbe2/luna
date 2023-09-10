#pragma once
#include <nodes/canvasItem.h>
namespace luna 
{
	namespace nodes 
	{
		class LN_API controlNode : public canvasItem
		{
		public:
			controlNode() = default;
			controlNode(entt::entity handle, luna::scene* scene);
			controlNode(luna::scene* scene);
			virtual ~controlNode() = default;
			virtual void init(luna::scene* scene) override;
			virtual void bindMethods() override;
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


