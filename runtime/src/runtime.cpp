#include <luna.h>
#include <runtimeLayer.h>
class runtime : public luna::application::application
{
public:
	runtime() //base function
	{
		LN_TRACE("started runtime");
		pushLayer(new luna::runtimeLayer());
	}
	virtual ~runtime() //base breaker function
	{
		
	}
};

luna::application::application* luna::application::createApplication()
{
	return new runtime(); // returns the application
}