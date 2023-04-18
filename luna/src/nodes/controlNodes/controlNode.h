#pragma once
#include <core/scene/scene.h>
namespace luna 
{
	namespace nodes 
	{
		class controlNode : public Node
		{
		public:
			controlNode() = default;
			controlNode(entt::entity handle, luna::scene* scene);
			controlNode(luna::scene* scene);
			virtual ~controlNode() = default;
			virtual void init(luna::scene* scene) override;
			virtual void guiEvent(Event& event);
			float getOffset();
			//rect2 getRect()
			void grabClickFocus();
			void grabFocus();
			bool hasFocus();
			void releaseFocus();
		};
	}
}


