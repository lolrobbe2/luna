#pragma once
#include <core/platform/platform.h>
namespace luna
{
	namespace application
	{
		class LN_API application
		{
		public:
			application();
			virtual ~application();
		};
		application* createApplication();
	}
}

