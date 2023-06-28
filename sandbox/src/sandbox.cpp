#pragma once
#include <core/platform/platformUtils.h>
#include <project/projectManager.h>
#include <project/visualStudio/projectGeneratorVS.h>
#include <luna.h>
class sandbox : public luna::application::application
{
public:
	sandbox() //base function
	{
		luna::project::projectGeneratorVS::generateProject("test","test");
		//luna::project::projectManager::init();
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