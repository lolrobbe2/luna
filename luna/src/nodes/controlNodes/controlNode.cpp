#include "controlNode.h"
#include <core/scripting/scriptingEngine.h>
#include <core/object/methodDB.h>
namespace luna 
{
	namespace nodes
	{
		/*-----------------------------------------------------------------------*/
		/*                                glue                                   */
		/*-----------------------------------------------------------------------*/

		static void ControlNodeGetPosition(entt::entity nodeId , glm::vec2* outTranslation)
		{
			*outTranslation = controlNode(nodeId, scripting::scriptingEngine::getContext()).getPosition();
		}

		static void ControlNodeSetPosition(entt::entity nodeId, glm::vec2* inPosition)
		{
			controlNode(nodeId, scripting::scriptingEngine::getContext()).setPosition(*inPosition);
		}





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
			addComponent<scriptComponent>();
			addComponent<transformComponent>();
			LN_CORE_INFO("node uuid = {0}", getUUID().getId());
		}
		void controlNode::bindMethods()
		{
			LN_ADD_INTERNAL_CALL(controlNode, ControlNodeGetPosition);
			LN_ADD_INTERNAL_CALL(controlNode, ControlNodeSetPosition);
		}
		void controlNode::guiEvent(Event& event) 
		{

		}
		glm::vec2 controlNode::getPosition()
		{
			return getComponent<transformComponent>().translation;
		}

		void controlNode::setPosition(glm::vec2 position) 
		{
			auto& transform = getComponent<transformComponent>();
			transform.translation.x = position.x;
			transform.translation.y = position.y;
		}
	}
}



