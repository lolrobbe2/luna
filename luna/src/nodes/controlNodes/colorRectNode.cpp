#include "colorRectNode.h"
#include <core/rendering/renderer2D.h>
namespace luna 
{ 
	namespace nodes
	{
		colorRectNode::colorRectNode(entt::entity handle, luna::scene* scene)
			: controlNode(handle, scene) {}
		colorRectNode::colorRectNode(luna::scene* scene)
		{
			this->scene = scene;
			entityHandle = scene->create();
			addComponent<idComponent>();
			addComponent<transformComponent>();
			LN_CORE_INFO("node uuid = {0}", getUUID().getId());
		}

		void colorRectNode::init(luna::scene* scene)
		{
			this->scene = scene;
			entityHandle = scene->create();
			addComponent<idComponent>().typeName = LN_CLASS_STRINGIFY(colorRectNode);
			addComponent<scriptComponent>();
			addComponent<transformComponent>();
			LN_CORE_INFO("node uuid = {0}", getUUID().getId());
		}
		void colorRectNode::setColor(const glm::vec4& color)
		{
			getComponent<canvasComponent>().modulate = color;
		}
		glm::vec4 colorRectNode::getColor()
		{
			return getComponent<canvasComponent>().modulate ;
		}
		void colorRectNode::draw()
		{
			auto& canvasComp = getComponent<canvasComponent>();
			auto& transform = getComponent<transformComponent>();
			renderer::renderer2D::drawQuad(transform.translation, transform.scale, canvasComp.modulate);
		}
	}
}
