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
			this->scene = scene;
			entityHandle = scene->create();
			addComponent<idComponent>().typeName = LN_CLASS_STRINGIFY(lineEditNode);
			addComponent<scriptComponent>();
			LN_CORE_INFO("node uuid = {0}", getUUID().getId());
			/*label Node Components*/
			addComponent<transformComponent>();
			addComponent<spriteRendererComponent>();
			addComponent<labelRendererComponent>();
		}
	}
}