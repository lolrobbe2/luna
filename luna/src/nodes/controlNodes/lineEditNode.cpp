#include "lineEditNode.h"
namespace luna
{
	namespace nodes
	{
		lineEditNode::lineEditNode(entt::entity handle, luna::scene* scene) : controlNode(handle, scene)
		{

		}
		lineEditNode::lineEditNode(luna::scene* scene) : controlNode(scene)
		{

		}

		void lineEditNode::init(luna::scene* scene)
		{
			controlNode::init(scene);
			LN_CLASS_TYPE_NAME(controlNode);
		}
	}
}