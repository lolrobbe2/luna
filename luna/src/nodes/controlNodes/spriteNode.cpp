#include "spriteNode.h"
#include <core/rendering/renderer2D.h>
namespace luna
{
	namespace nodes
	{
		spriteNode::spriteNode(entt::entity handle, luna::scene* scene) : canvasItem(handle,scene)
		{

		}
		spriteNode::spriteNode(luna::scene* scene) : canvasItem(scene)
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
			LN_CORE_INFO("scene ptr: {0}",(void*)scene);

			addComponent<canvasComponent>().drawFunction = LN_DRAW_LAMBDA(spriteNode);
			LN_CORE_INFO("node uuid = {0}", getUUID().getId());
			/*sprite Node Components*/
			addComponent<transformComponent>();
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
