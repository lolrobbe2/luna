#include "labelNode.h"
namespace luna
{
	namespace nodes
	{
		labelNode::labelNode(entt::entity handle, luna::scene* scene) : controlNode(handle, scene)
		{

		}
		labelNode::labelNode(luna::scene* scene) : controlNode(scene)
		{
			addComponent<transformComponent>();
			addComponent<labelRendererComponent>();
		}

		void labelNode::init(luna::scene* scene) 
		{
			this->scene = scene;
			entityHandle = scene->create();
			addComponent<idComponent>();
			LN_CORE_INFO("node uuid = {0}", getUUID().getId());
			/*label Node Components*/
			addComponent<transformComponent>();
			addComponent<labelRendererComponent>();
		}
	}
}

