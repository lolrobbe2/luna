#include "spriteNode.h"
namespace luna
{
	namespace nodes
	{
		spriteNode::spriteNode(entt::entity handle, luna::scene* scene) : Node(handle,scene)
		{

		}
		spriteNode::spriteNode(luna::scene* scene) : Node(scene)
		{
			addComponent<transformComponent>();
			addComponent<spriteRendererComponent>();
		}
	}
}
