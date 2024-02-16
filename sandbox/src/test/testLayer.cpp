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
		
	}
	void testLayer::onImGuiRender()
	{
	}
	void testLayer::onEvent(Event& event)
	{
	}
}
