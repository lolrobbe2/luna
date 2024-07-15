#include "spriteNode.h"
#include <core/application.h>
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
			controlNode::init(scene);
			LN_CLASS_TYPE_NAME(spriteNode);
			LN_CANVAS_COMPONENT(spriteNode);
			LN_CORE_INFO("node uuid = {0}", getUUID().getId());
			/*sprite Node Components*/
			addComponent<spriteRendererComponent>();
		}
		void spriteNode::draw()
		{
			
			auto& sprite = getComponent<spriteRendererComponent>();
			auto& transform = getComponent<transformComponent>();

			if(sprite.texture) RENDERER->drawQuad(transform.getTransform(), sprite.texture);
		}
	}
}
