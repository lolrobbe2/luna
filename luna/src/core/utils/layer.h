#pragma once
/*from hazel2D*/

#include <core/utils/timestep.h>
#include <core/events/event.h>
#include <core/events/keyEvent.h>
#include "lnpch.h"

namespace luna 
{
	namespace utils
	{
		class LN_API layer
		{
		public:
			layer(const std::string& name = "luna layer");
			virtual ~layer() = default;

			virtual void onAttach() {}
			virtual void onDetach() {}
			virtual void onUpdate(timestep ts) {}
			virtual void onEvent(Event& event) {}
		#ifndef DISABLE_IMGUI
			virtual void onImGuiRender() {}
		#endif //!DISABLE_IMGUI
			const std::string& getName() const { return m_DebugName; }
		protected:
			std::string m_DebugName;
		};
	}
}