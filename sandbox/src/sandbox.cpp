#pragma once
#include <luna.h>
class sandbox : public luna::application::application
{
public:
	sandbox() //base function
	{
		LN_TRACE("started app");
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