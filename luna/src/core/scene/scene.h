#pragma once
#include "lnpch.h"
#include <entt.h>
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
		
		entt::entity create() 
		{
			return m_Registry.create();
		}
	private:
		friend class Node;
		friend class sceneHierarchyPanel;
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
		Node(scene* scene);
		virtual ~Node() = default;
		void setName(std::string name);
		void addChild(Node node);
		virtual void init(scene* scene) override;

		friend class scene;
		friend class sceneHierarchyPanel;

		template<typename T, typename... Args>
		T& addComponent(Args&&... args)
		{
			//LN_CORE_ASSERT(!hasComponent<T>(), "Node already has component!");
			T& component = scene->m_Registry.emplace<T>(entityHandle, std::forward<Args>(args)...);
			scene->onComponentAdded<T>(*this, component);
			return component;
		}
		template<typename T, typename... Args>
		T& addOrReplaceComponent(Args&&... args)
		{
			T& component = scene->m_Registry.emplace_or_replace<T>(entityHandle, std::forward<Args>(args)...);
			scene->onComponentAdded<T>(*this, component);
			return component;
		}
		template<typename T>
		T& getComponent()
		{
			return scene->m_Registry.get<T>(entityHandle);
		}
		template<typename T>
		bool hasComponent()
		{
			return scene->m_Registry.all_of<T>(entityHandle);
		}
		template<typename T>
		void removeComponent()
		{
			//HZ_CORE_ASSERT(HasComponent<T>(), "Node does not have component!");
			scene->m_Registry.remove<T>(entityHandle);
		}

		operator bool() const { return entityHandle != entt::null; }
		operator entt::entity() const { return entityHandle; }
		operator uint32_t() const { return (uint32_t)entityHandle; }

		uuid getUUID() { return getComponent<idComponent>().id; }
		const std::string& getName() {
			if(hasComponent<tagComponent>()) return getComponent<tagComponent>().tag; 
			else return addComponent<tagComponent>().tag;
		}

		bool operator==(const Node& other) const
		{
			return entityHandle == other.entityHandle && scene == other.scene;
		}

		bool operator!=(const Node& other) const
		{
			return !(*this == other);
		}
	protected:
		entt::entity entityHandle{ entt::null };
		scene* scene = nullptr;
	};
	
}


