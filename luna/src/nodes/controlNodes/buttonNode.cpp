#include "buttonNode.h"
namespace luna
{
	namespace nodes
	{
		buttonNode::buttonNode(entt::entity handle, luna::scene* scene) : spriteNode(handle, scene)
		{

		}
		buttonNode::buttonNode(luna::scene* scene) : spriteNode(scene)
		{
			getComponent<spriteRendererComponent>().showInEditor = false;
		}
	}
}
