
#pragma once
#include <editorLayer.h>
#include <projectLayer.h>
#include <luna.h>
class apollo : public luna::application::application
{
public:
	apollo() //base function
	{
		LN_TRACE("started apollo editor");
		pushLayer(new luna::projectLayer());
	}
	virtual ~apollo() //base breaker function
	{
		
	}

};

luna::application::application* luna::application::createApplication()
{
	return new apollo(); // returns the application
}

