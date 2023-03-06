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
		};
	}
}


