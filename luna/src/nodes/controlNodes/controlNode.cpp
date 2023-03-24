#include "controlNode.h"
namespace luna 
{
	namespace nodes
	{
		controlNode::controlNode(entt::entity handle, luna::scene* scene)
			: Node(handle,scene)
		{

		}
		controlNode::controlNode(luna::scene* scene)
		{
			this->scene = scene;
			entityHandle = scene->create();
			addComponent<idComponent>();
			addComponent<transformComponent>();
			LN_CORE_INFO("node uuid = {0}", getUUID().getId());
		}

		void controlNode::init(luna::scene* scene)
		{
			this->scene = scene;
			entityHandle = scene->create();
			addComponent<idComponent>().typeName = LN_CLASS_STRINGIFY(controlNode);
			addComponent<transformComponent>();
			LN_CORE_INFO("node uuid = {0}", getUUID().getId());
		}
		void controlNode::guiEvent(Event& event) 
		{

		}
	}
}
