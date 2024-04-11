#include "testLayer.h"
namespace luna
{
	testLayer::testLayer(const std::string& name)
	{
		this->m_DebugName = name;
	}
	void testLayer::onAttach()
	{
		LN_INFO("added testLayer");
	}
	void testLayer::onDetach()
	{
	}
	void testLayer::onUpdate(utils::timestep ts)
	{
		//application::application::get().getRenderer()->drawQuad(glm::mat4(1.0f), { 1.0f,1.0f,0.0f,0.0f });
		
		const glm::vec3 pos = { -1.0,-1.0,0.0 };
		const glm::vec2 size = { 0.1, 0.1 };
		const glm::vec4 color = { 1.0f, 1.0f, 0.0f, 0.0f };
		const glm::vec3 pos2 = { -0.9,-0.1,0.0 };

		const glm::vec4 color2 = { 0.0f, 1.0f, 0.0f, 0.0f };
		application::application::get().getRenderer()->drawQuad(pos, size, color);
		//application::application::get().getRenderer()->drawQuad(glm::mat4(1.0f), { 1.0f,1.0f,0.0f,0.0f });
		application::application::get().getRenderer()->drawQuad(pos2, size, color);

		
	


	}
	void testLayer::onImGuiRender()
	{
	}
	void testLayer::onEvent(Event& event)
	{
	}
}
