#include "labelNode.h"
namespace luna
{
	namespace nodes
	{
		labelNode::labelNode(entt::entity handle, luna::scene* scene) : Node(handle, scene)
		{

		}
		labelNode::labelNode(luna::scene* scene) : Node(scene)
		{
			addComponent<transformComponent>();
			addComponent<labelRendererComponent>();
		}
	}
}

