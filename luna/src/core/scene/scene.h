#pragma once
#include "lnpch.h"
#include <entt.h>
#include <core/utils/objectStorage.h>
namespace luna 
{
	class scene
	{
	public:
		template<typename T>
		T& addNode(std::string name = std::string());
		template<typename T>
		bool deleteNode(const T& node);

	private:
		entt::registry m_Registry;
		uint32_t m_ViewportWidth = 0, m_ViewportHeight = 0;
		bool m_IsRunning = false;
		bool m_IsPaused = false;
		int m_StepFrames = 0;
		utils::objectStorage<entt::entity> enttityStorage;

	};
}


