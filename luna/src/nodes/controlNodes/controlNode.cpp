#include "controlNode.h"
#include <core/scripting/scriptingEngine.h>
#include <core/object/methodDB.h>
#include <core/scene/node.h>

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

		static void ControlNodeGetScale(entt::entity nodeId, glm::vec2* outScale)
		{
			*outScale = controlNode(nodeId, scripting::scriptingEngine::getContext()).getScale();
		}

		static void ControlNodeSetScale(entt::entity nodeId, glm::vec2* inScale)
		{
			controlNode(nodeId, scripting::scriptingEngine::getContext()).setScale(*inScale);
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
			addComponent<signalComponent>();
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
			LN_ADD_INTERNAL_CALL(controlNode, ControlNodeGetScale);
			LN_ADD_INTERNAL_CALL(controlNode, ControlNodeSetScale);
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

		glm::vec2 controlNode::getScale()
		{
			return getComponent<transformComponent>().scale;
		}

		void controlNode::setScale(glm::vec2 position)
		{
			auto& transform = getComponent<transformComponent>();
			transform.scale.x = position.x;
			transform.scale.y = position.y;
		}
	}
}



