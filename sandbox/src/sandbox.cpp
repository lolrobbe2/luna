#pragma once
#include <project/projectManager.h>
#include <luna.h>
class sandbox : public luna::application::application
{
public:
	sandbox() //base function
	{
		luna::project::projectManager::init();
	}
	virtual ~sandbox() //base breaker function
	{
		
	}

};

luna::application::application* luna::application::createApplication()
{
	//hello
	return new sandbox(); // returns the application
}