#pragma once
#include "lnpch.h"
#include <entt.h>
#include <core/utils/objectStorage.h>
#include <core/scene/baseComponents.h>
namespace luna 
{
	class node;
	class LN_API scene
	{
	public:
		scene() = default;
		template<typename T>
		T& addNode(std::string name = std::string())
		{

			T node{ this };
			LN_CORE_INFO("added node, type = {0}", typeid(T).name());
			node.setName(name);
			// TODO: insert return statement here
			return node;
		};
		template<typename T>
		bool deleteNode(const T& node);

		template<typename T>
		void onComponentAdded(node node, T& component);
	private:
		friend class node;
		entt::registry m_Registry;
		uint32_t m_ViewportWidth = 0, m_ViewportHeight = 0;
		bool m_IsRunning = false;
		bool m_IsPaused = false;
		int m_StepFrames = 0;
		utils::objectStorage<entt::entity> enttityStorage;

	};
	
	class node 
	{
	public:
		node(scene* scene);
		virtual ~node() = default;
		void setName(std::string name);
	protected:

		template<typename T, typename... Args>
		T& addComponent(Args&&... args);
		template<typename T, typename... Args>
		T& addOrReplaceComponent(Args&&... args);
		template<typename T>
		T& getComponent();
		template<typename T>
		bool hasComponent();

		template<typename T>
		void removeComponent()
		{
			//HZ_CORE_ASSERT(HasComponent<T>(), "node does not have component!");
			scene->m_Registry.remove<T>(entityHandle);
		}

		operator bool() const { return entityHandle != entt::null; }
		operator entt::entity() const { return entityHandle; }
		operator uint32_t() const { return (uint32_t)entityHandle; }

		uuid getUUID() { return getComponent<idComponent>().id; }
		const std::string& GetName() { return getComponent<tagComponent>().tag; }

		bool operator==(const node& other) const
		{
			return entityHandle == other.entityHandle && scene == other.scene;
		}

		bool operator!=(const node& other) const
		{
			return !(*this == other);
		}
	protected:
		entt::entity entityHandle{ entt::null };
		scene* scene;
	};
	
}


