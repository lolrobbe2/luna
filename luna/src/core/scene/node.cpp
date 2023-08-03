#include <core/scene/node.h>
#include <core/events/mouseEvent.h>
#include <nodes/controlNodes/itemListNode.h>
#include <core/object/methodDB.h>
#include <core/scripting/scriptingEngine.h>
#include <queue>
namespace luna 
{
#pragma region NODE

	/*-----------------------------------------------------------------------*/
	/*                                glue                                 */
	/*-----------------------------------------------------------------------*/


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

		MonoArray* nodeArray = scripting::scriptingEngine::createArray<Node>(children.size());
		for (size_t i = 0; i < children.size(); i++)
		{
			auto& script = children[i].getComponent<scriptComponent>();
			MonoObject* nodeObject = script.scritpInstance->getInstance();
			mono_array_set(nodeArray, MonoObject*, i, nodeObject);
		}

		return nodeArray;
	}

	static MonoObject* NodeGetParent(entt::entity nodeId)
	{
		Node node = { nodeId,scripting::scriptingEngine::getContext() };
		Node parent = node.getParent();
		if (parent) {
			return parent.getComponent<scriptComponent>().scritpInstance->getInstance();
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
	Node::Node(uint64_t id, luna::scene* scene)
	{
		auto idComponents = scene->m_Registry.group<idComponent, tagComponent>();
		this->scene = scene;
		if (id == -1) return;
		for (auto entity : idComponents)
		{
			auto [testId, tag] = scene->m_Registry.get<idComponent, tagComponent>(entity);
			if (testId.id == id) {
				entityHandle = entity;
				if (!hasComponent<idComponent>()) addComponent<idComponent>().id = id;
				return;
			}
		}
	}

	Node::Node(entt::entity handle, luna::scene* scene)
		: object(handle, scene)
	{
	}

	Node::Node(luna::scene* scene)
	{
		this->scene = scene;
		entityHandle = scene->create();
		addComponent<idComponent>().typeName = LN_CLASS_STRINGIFY(Node);
		addComponent<scriptComponent>();
		LN_CORE_INFO("node uuid = {0}", getUUID().getId());
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
				children.push_back(Node(child, scene));
			}
		}
		return children;
	}

	Node Node::getParent()
	{
		if (hasComponent<parentComponent>()) {
			auto& parentComp = getComponent<parentComponent>();
			Node parent{ parentComp.parentId,scene };
			return parent;
		}
		return Node(-1, scene);
	}

	void Node::init(luna::scene* scene)
	{
		this->scene = scene;
		entityHandle = scene->create();

		addComponent<idComponent>().typeName = LN_CLASS_STRINGIFY(Node);
		addComponent<scriptComponent>();
		LN_CORE_INFO("node uuid = {0}", getUUID().getId());
	}

	void Node::bindMethods()
	{
		LN_ADD_INTERNAL_CALL(Node, NodeSetName);
		LN_ADD_INTERNAL_CALL(Node, NodeGetName);
		LN_ADD_INTERNAL_CALL(Node, NodeGetChildren);
		LN_ADD_INTERNAL_CALL(Node, NodeGetParent);
		LN_ADD_INTERNAL_CALL(Node, NodeAddSibling);
		LN_ADD_INTERNAL_CALL(Node, NodeAddChild);
		LN_ADD_INTERNAL_CALL(Node, NodeCreateNew);
	}

#pragma endregion
}