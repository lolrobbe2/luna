#include "colorRectNode.h"
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
			addComponent<rectComponent>();
			LN_CORE_INFO("node uuid = {0}", getUUID().getId());
		}

		void colorRectNode::init(luna::scene* scene)
		{
			this->scene = scene;
			entityHandle = scene->create();
			addComponent<idComponent>().typeName = LN_CLASS_STRINGIFY(colorRectNode);
			addComponent<transformComponent>();
			addComponent<rectComponent>();
			LN_CORE_INFO("node uuid = {0}", getUUID().getId());
		}
		void colorRectNode::setColor(const glm::vec4& color)
		{
			getComponent<rectComponent>().color = color;
		}
		glm::vec4 colorRectNode::getColor()
		{
			return getComponent<rectComponent>().color;
		}
	}
}
