#include <core/scene/scene.h>
#include <core/scene/node.h>
#include <core/events/mouseEvent.h>
#include <nodes/controlNodes/itemListNode.h>
#include <core/object/methodDB.h>
#include <core/scripting/scriptingEngine.h>
#include <queue>
#include <core/scripting/monoArray.h>
#include <core/scripting/monoObject.h>
namespace luna 
{
#pragma region NODE

	/*-----------------------------------------------------------------------*/
	/*                                glue                                 */
	/*-----------------------------------------------------------------------*/
	/*

	static void NodeSetName(entt::entity nodeHandle, MonoString* name)
	{
		Node node = { nodeHandle,scripting::scriptingEngine::getContext() };
		node.setName(mono_string_to_utf8(name));
	}

	static void NodeGetName(entt::entity nodeHandle, MonoString** name)
	{
		Node node = { nodeHandle,scripting::scriptingEngine::getContext() };
		*name = scripting::scriptingEngine::createMonoString(node.getName());
	}
	static MonoArray* NodeGetChildren(entt::entity nodeId)
	{
		Node node = { nodeId,scripting::scriptingEngine::getContext() };
		auto children = node.getChildren();

		//MonoArray* nodeArray = 
		luna::scripting::monoArray nodes(scripting::scriptingEngine::createArray<Node>(children.size()));
		
		LN_UNROLL_LOOP
		for (size_t i = 0; i < children.size(); i++)
		{
			auto& script = children[i].getComponent<scriptComponent>();
			luna::scripting::monoObject nodeObject{ script.scritpInstance->getInstance() };
			nodes[i] = nodeObject;
		}
		
		return nodes.getNative();
	}

	static MonoObject* NodeGetParent(entt::entity nodeId)
	{
		Node node = { nodeId,scripting::scriptingEngine::getContext() };
		Node parent = node.getParent();
		if (parent) {
			return parent.getComponent<scriptComponent>().scritpInstance->getInstance().getNative();
		}
		return nullptr;
	}

	static void NodeAddSibling(entt::entity nodeId, entt::entity siblingId)
	{
		Node sibling = { nodeId,scripting::scriptingEngine::getContext() };
		sibling.getParent().addChild(Node(siblingId, scripting::scriptingEngine::getContext()));
	}

	static void NodeAddChild(entt::entity nodeId, entt::entity childId)
	{
		Node parent = { nodeId,scripting::scriptingEngine::getContext() };
		parent.addChild(Node(childId, scripting::scriptingEngine::getContext()));
	}

	static entt::entity NodeCreateNew()
	{
		return Node(scripting::scriptingEngine::getContext());
	}
	*/
	Node::Node(uint64_t id, luna::scene* scene)
		: object(id,scene)
	{
		
	}

	Node::Node(entt::entity handle, luna::scene* scene)
		: object(handle, scene)
	{
	}

	Node::Node(luna::scene* scene)
	{
		init(scene);

	}

	void Node::setName(std::string name)
	{
		if (hasComponent<tagComponent>()) getComponent<tagComponent>().tag = name;
		else addComponent<tagComponent>(name);

	}

	void Node::addChild(Node node)
	{
		LN_CORE_INFO("adding node {0} as a child to {1} .", node.getUUID().getId(), getUUID().getId());
		if (node.hasComponent<parentComponent>()) node.getComponent<parentComponent>().parentId = getComponent<idComponent>().id;
		else node.addComponent<parentComponent>().parentId = getComponent<idComponent>().id;

		if (hasComponent<childComponent>()) getComponent<childComponent>().childs.push_back(node);
		else addComponent<childComponent>().childs.push_back(node);
	}

	std::vector<Node> Node::getChildren()
	{
		std::vector<Node> children;
		if (hasComponent<childComponent>())
		{
			auto childrenID = getComponent<childComponent>().childs;
			for (auto child : childrenID) {
				children.push_back(Node(child, p_scene));
			}
		}
		return children;
	}

	Node Node::getParent()
	{
		if (hasComponent<parentComponent>()) {
			auto& parentComp = getComponent<parentComponent>();
			Node parent{ parentComp.parentId,p_scene };
			return parent;
		}
		return Node(-1, p_scene);
	}

	void Node::init(luna::scene* scene)
	{
		this->p_scene = scene;
		entityHandle = scene->create();
		addComponent<idComponent>().typeName = LN_CLASS_STRINGIFY(Node);
		addComponent<scriptComponent>();
		addComponent<signalComponent>(); 
	}

	void Node::bindMethods()
	{
		methodDB::registerMethod(METHOD_DEF("SetName", "name"), &setName);
		methodDB::registerMethod(METHOD_DEF("GetName"),&getName);
		methodDB::registerMethod(METHOD_DEF("GetChildren"), &getChildren);
		/*
		LN_ADD_INTERNAL_CALL(Node, NodeSetName);
		LN_ADD_INTERNAL_CALL(Node, NodeGetName);
		LN_ADD_INTERNAL_CALL(Node, NodeGetChildren);
		LN_ADD_INTERNAL_CALL(Node, NodeGetParent);
		LN_ADD_INTERNAL_CALL(Node, NodeAddSibling);
		LN_ADD_INTERNAL_CALL(Node, NodeAddChild);
		LN_ADD_INTERNAL_CALL(Node, NodeCreateNew);
		*/
	}

#pragma endregion
}