#include "spriteNode.h"
#include <core/rendering/renderer2D.h>
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
			LN_CORE_INFO("scene ptr: {0}",(void*)scene);
			auto& sprite = getComponent<spriteRendererComponent>();
			auto& transform = getComponent<transformComponent>();

			if (sprite.texture) sprite.outOfBounds = renderer::renderer2D::drawQuad(transform.translation, { transform.scale.x,transform.scale.y }, sprite.texture);
		}
	}
}
