#pragma once
#include <core/application.h>

extern luna::application::application*  luna::application::createApplication();
int main(int argc, char** argv)
{

	/*note put all code in run loop*/
	luna::application::application* app = luna::application::createApplication(); // creates an app instance
	app->run(); // starts the instance
	delete app; // removes the instance when the app is closed

}