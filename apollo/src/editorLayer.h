#pragma once
#include <core/utils/layer.h>
#include <core/scene/scene.h>
namespace luna
{
	class editorLayer : public utils::layer
	{
	public:
		editorLayer(const std::string& name = "editor layer");
		virtual ~editorLayer() = default;
		
		virtual void onAttach() override;
		virtual void onDetach()  override;
		virtual void onUpdate(utils::timestep ts)  override;
		virtual void onImGuiRender()  override;
		virtual void onEvent(Event& event) override;
	private:
		ref<scene> activeScene;
	};
}


