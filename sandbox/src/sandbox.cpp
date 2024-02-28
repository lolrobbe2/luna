#pragma once
#include <luna.h>
#include <test/testLayer.h>
class sandbox : public luna::application::application
{
public:
	sandbox() //base function
	{

		pushLayer(new luna::testLayer());
		//luna::project::projectManager::init();
	}
	virtual ~sandbox() //base breaker function
	{
		
	}

};

luna::application::application* luna::application::createApplication()
{
	return new sandbox(); // returns the application
}