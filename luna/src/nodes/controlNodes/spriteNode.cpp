#include "spriteNode.h"
namespace luna
{
	namespace nodes
	{
		spriteNode::spriteNode(entt::entity handle, luna::scene* scene) : controlNode(handle,scene)
		{

		}
		spriteNode::spriteNode(luna::scene* scene) : controlNode(scene)
		{
			addComponent<transformComponent>();
			addComponent<spriteRendererComponent>();
		}

		void spriteNode::init(luna::scene* scene)
		{
			this->scene = scene;
			entityHandle = scene->create();
			addComponent<idComponent>().typeName = LN_CLASS_STRINGIFY(spriteNode);
			addComponent<scriptComponent>();
			LN_CORE_INFO("node uuid = {0}", getUUID().getId());
			/*sprite Node Components*/
			addComponent<transformComponent>();
			addComponent<spriteRendererComponent>();
		}
	}
}
