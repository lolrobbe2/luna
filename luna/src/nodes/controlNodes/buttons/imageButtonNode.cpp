#include "imageButtonNode.h"
namespace luna 
{
	namespace nodes
	{
		imageButtonNode::imageButtonNode(entt::entity handle, luna::scene* scene) : buttonNode(handle, scene)
		{

		}
		imageButtonNode::imageButtonNode(luna::scene* scene) : buttonNode(scene)
		{
			getComponent<spriteRendererComponent>().showInEditor = false;
			addComponent<buttonComponent>();

		}
		void imageButtonNode::init(luna::scene* scene)
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
