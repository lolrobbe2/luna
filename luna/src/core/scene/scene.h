#pragma once
#include "lnpch.h"
#include <core/utils/objectStorage.h>
#include <core/scene/baseComponents.h>
#include <core/utils/timestep.h>

namespace luna 
{
	class LN_API object;
	class LN_API Node;
	class LN_API scene
	{
	public:
		scene() = default;
		~scene();
		template<typename T>
		T& addNode(std::string name = std::string());

		template<typename T>
		bool destroyNode(const T& Node);

		template<typename T>
		void onComponentAdded(Node Node, T& component);

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
		bool isRunning() { return m_IsRunning; }
	protected:
		friend class Node;
		friend class object;
		friend class sceneSerializer;
		friend class sceneHierarchyPanel;
		friend class editorLayer;
		friend class scriptingEngine;
		entt::registry m_Registry;
		uint32_t m_ViewportWidth = 0, m_ViewportHeight = 0;
		bool m_IsRunning = false;
		bool m_IsPaused = false;
		int m_StepFrames = 0;
		utils::objectStorage<entt::entity> enttityStorage;

	};
		
}


