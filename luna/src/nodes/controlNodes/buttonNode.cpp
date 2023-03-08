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
		void buttonNode::init(luna::scene* scene)
		{
			this->scene = scene;
			entityHandle = scene->create();
			addComponent<idComponent>();
			LN_CORE_INFO("node uuid = {0}", getUUID().getId());
			/*sprite Node Components*/
			addComponent<transformComponent>();
			addComponent<spriteRendererComponent>().showInEditor = false;
			addComponent<buttonComponent>();
		}
	}
}
