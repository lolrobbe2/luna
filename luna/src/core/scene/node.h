#ifndef _NODE_
#define _NODE_
#include <core/object/objectDB.h>
#include <core/object/object.h>
namespace luna 
{
	class LN_API scene;
	class LN_API Node : public luna::object
	{
	public:
		LN_CLASS(Node, object);
		void setName(std::string name);
		void addChild(Node node);
		std::vector<Node> getChildren();
		Node getParent();

		friend class scene;
		friend class sceneHierarchyPanel;
		entt::entity getQuikId() { return entityHandle; };
		luna::scene* getScene() { return p_scene; };
		operator bool() const { return entityHandle != entt::null; }
		operator entt::entity() const { return entityHandle; }
		operator uint32_t() const { return (uint32_t)entityHandle; }
		operator luna::scene* () const { return p_scene; };
		std::string& getName() {
			return getComponent<tagComponent>().tag;
		}

		bool operator==(const Node& other) const
		{
			return entityHandle == other.entityHandle && p_scene == other.p_scene;
		}

		bool operator!=(const Node& other) const
		{
			return !(*this == other);
		}
	};

}

#endif