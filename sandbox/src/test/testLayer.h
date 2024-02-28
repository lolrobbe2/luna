#pragma once
#include <core/utils/layer.h>
#include <core/application.h>
namespace luna
{
	class testLayer : public luna::utils::layer
	{
	public:
		testLayer(const std::string& name = "test layer");
		virtual ~testLayer() = default;

		virtual void onAttach() override;
		virtual void onDetach() override;
		virtual void onUpdate(utils::timestep ts) override;
		virtual void onImGuiRender() override;
		virtual void onEvent(Event& event) override;
	};
}

