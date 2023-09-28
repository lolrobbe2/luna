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
			controlNode::init(scene);
			LN_TYPE_TO_NAME(labelNode);
			LN_CANVAS_COMPONENT(labelNode);
			addComponent<labelRendererComponent>();
		}
		void labelNode::draw()
		{
			auto& label = getComponent<labelRendererComponent>();
			if (label.font) drawString(label.font, label.pos,label.text, label.fontSize, label.color);//drawLabel(transform.translation, { transform.scale.x,transform.scale.y }, label.font, label.text);
		}
	}
}

