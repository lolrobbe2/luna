#pragma once
#include "lnpch.h"
#include <core/utils/objectStorage.h>
#include <core/scene/baseComponents.h>
#include <core/utils/timestep.h>
#include <core/object/objectDB.h>
namespace luna 
{
	class LN_API Node;
	class LN_API scene
	{
	public:
		scene() = default;
		~scene();
		template<typename T>
		T& addNode(std::string name = std::string())
		{

			T Node{ this };
			LN_CORE_INFO("added Node, type = {0}", typeid(T).name());
			Node.setName(name);
			// TODO: insert return statement here
			return Node;
		};
		template<typename T>
		bool destroyNode(const T& Node);

		template<typename T>
		void onComponentAdded(Node Node, T& component)
		{
			
		}

		template<typename... components>
		auto getAllEntitiesWith()
		{
			return m_Registry.view<components...>();
		}
		void onUpdateEditor(utils::timestep ts);
		
		void onUpdate(utils::timestep ts);
		entt::entity create() 
		{
			return m_Registry.create();
		}

		void onEvent(Event& event);
		void onPlayScene();
		void onStopScene();
		void resetScriptComponent();
		void process(utils::timestep ts);
	private:
		friend class Node;
		friend class object;
		friend class sceneSerializer;
		friend class sceneHierarchyPanel;
		friend class editorLayer;
		entt::registry m_Registry;
		uint32_t m_ViewportWidth = 0, m_ViewportHeight = 0;
		bool m_IsRunning = false;
		bool m_IsPaused = false;
		int m_StepFrames = 0;
		utils::objectStorage<entt::entity> enttityStorage;

	};
	
	class LN_API Node : public object
	{
	public:

		Node() = default;
		Node(entt::entity handle, luna::scene* scene);
		Node(uint64_t id, luna::scene* scene);
		Node(luna::scene* scene);
		virtual ~Node() = default;
		void setName(std::string name);
		void addChild(Node node);
		std::vector<Node> getChildren();
		Node getParent();
		
		virtual void init(luna::scene* scene) override;
		virtual void bindMethods() override;
		friend class scene;
		friend class sceneHierarchyPanel;

		operator bool() const { return entityHandle != entt::null; }
		operator entt::entity() const { return entityHandle; }
		operator uint32_t() const { return (uint32_t)entityHandle; }
		operator luna::scene* () const { return scene; };
		uuid getUUID() { return getComponent<idComponent>().id; }
		std::string& getName() {
			return getComponent<tagComponent>().tag;
		}

		bool operator==(const Node& other) const
		{
			return entityHandle == other.entityHandle && scene == other.scene;
		}

		bool operator!=(const Node& other) const
		{
			return !(*this == other);
		}
	};
	
}


