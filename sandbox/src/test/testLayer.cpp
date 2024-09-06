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
		const glm::vec4 color = { 1.0f, 0.0f, 0.0f, 0.0f };
		const glm::vec3 pos2 = { -0.9,-0.1,0.0 };

		const glm::vec4 color2 = { 0.0f, 1.0f, 0.0f, 0.0f };

		const glm::vec3 pos3 = { -0.8,-0.5,0.0 };

		const glm::vec4 color3 = { 0.0f, 0.0f, 1.0f, 0.0f };
		application::application::get().getRenderer()->drawQuad(pos, size, color);
		//application::application::get().getRenderer()->drawQuad(glm::mat4(1.0f), { 1.0f,1.0f,0.0f,0.0f });

		application::application::get().getRenderer()->drawQuad(pos2, size, color2);
		application::application::get().getRenderer()->drawQuad(pos3, size, color3);
		

		/*
		const glm::vec2 size = { 0.1, 0.1 };
		const int numSquares = 20;
		const float halfWidth = size.x * numSquares * 0.5f;
		const float halfHeight = size.y * numSquares * 0.5f;

		// Loop through rows
		for (int i = 0; i < numSquares; ++i) {
			// Loop through columns
			for (int j = 0; j < numSquares; ++j) {
				glm::vec3 pos = { -halfWidth + size.x * j, -halfHeight + size.y * i, 0.0 };
				glm::vec4 color;

				// Alternate colors based on even/odd position
				if ((i + j) % 2 == 0) {
					color = { 1.0f, 0.0f, 0.0f, 0.0f }; // Red
				}
				else {
					color = { 0.0f, 1.0f, 0.0f, 0.0f }; // Green
				}

				// Draw each square
				application::application::get().getRenderer()->drawQuad(pos, size, color);
			}
		}
		*/
	}
	void testLayer::onImGuiRender()
	{
	}
	void testLayer::onEvent(Event& event)
	{
	}
}
