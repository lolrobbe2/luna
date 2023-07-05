#pragma once
#include <core/utils/layer.h>
#include <core/utils/timestep.h>
//DISCLAIMER: THIS SCREEN HAS BEEN INSPIRED BY THE GODOT PROJECTS SCREEN!
namespace luna
{
	class projectLayer : public utils::layer
	{
	public:
		projectLayer(const std::string& name = "luna layer");
		projectLayer(utils::layer* layer, const std::string& name = "luna layer");
		virtual ~projectLayer() = default;

		virtual void onAttach();
		virtual void onDetach();
		virtual void onUpdate(utils::timestep ts);
		virtual void onImGuiRender();
		virtual void onEvent(Event& event);

	private:
		void inputText(const std::string& name, std::string& stringBuffer,float width = 0.0f, const std::string& hint = "");
		utils::layer* _editorLayer = nullptr;
	};
}