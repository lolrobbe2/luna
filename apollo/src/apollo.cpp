
#pragma once
#include <editorLayer.h>
#include <luna.h>
class apollo : public luna::application::application
{
public:
	apollo() //base function
	{
		LN_TRACE("started apollo editor");
		pushLayer(new luna::editorLayer());
	}
	virtual ~apollo() //base breaker function
	{
		
	}

};

luna::application::application* luna::application::createApplication()
{
	return new apollo(); // returns the application
}

