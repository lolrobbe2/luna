#pragma once
/*from hazel2D*/

#include <core/utils/timestep.h>
#include <core/events/event.h>
#include "lnpch.h"

namespace luna 
{
	namespace utils
	{
		class layer
		{
		public:
			layer(const std::string& name = "luna layer");
			virtual ~layer() = default;

			virtual void onAttach() {}
			virtual void onDetach() {}
			virtual void onUpdate(timestep ts) {}
			virtual void onImGuiRender() {}
			virtual void onEvent(Event& event) {}

			const std::string& getName() const { return m_DebugName; }
		protected:
			std::string m_DebugName;
		};
	}
}