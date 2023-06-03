#pragma once
#define DISABLE_IMGUI
#include <core/utils/layer.h>
#include <core/scene/scene.h>
#include <core/events/keyEvent.h>
namespace luna
{
	class runtimeLayer : public utils::layer
	{
	public:
		runtimeLayer(const std::string& name = "runtime layer");
		virtual ~runtimeLayer() = default;
		
		virtual void onAttach() override;
		virtual void onDetach() override;
		virtual void onUpdate(utils::timestep ts) override;
		virtual void onEvent(Event& event) override;
	private:
		ref<scene> startupScene;
	};
}


