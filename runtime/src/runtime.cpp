#define DISABLE_IMGUI
#include <luna.h>
class runtime : public luna::application::application
{
public:
	runtime() //base function
	{
		LN_TRACE("started runtime");
		//pushLayer(new luna::runtime());
	}
	virtual ~runtime() //base breaker function
	{

	}

};

luna::application::application* luna::application::createApplication()
{
	return new runtime(); // returns the application
}