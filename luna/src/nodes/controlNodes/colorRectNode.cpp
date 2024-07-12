#include "colorRectNode.h"
#include <core/application.h>
namespace luna 
{ 
	namespace nodes
	{
		colorRectNode::colorRectNode(entt::entity handle, luna::scene* scene)
			: controlNode(handle, scene) {}
		colorRectNode::colorRectNode(luna::scene* scene)
		{
			controlNode::init(scene);
			addComponent<idComponent>();
			LN_CORE_INFO("node uuid = {0}", getUUID().getId());
		}

		void colorRectNode::init(luna::scene* scene)
		{
			controlNode::init(scene);
			LN_CLASS_TYPE_NAME(controlNode);
			LN_CANVAS_COMPONENT();
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
			RENDERER->drawQuad(transform.translation, transform.scale, canvasComp.modulate);
		}
	}
}
