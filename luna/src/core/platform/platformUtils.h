#pragma once
#include "lnpch.h"
#include <core/core.h>
namespace luna 
{
	namespace platform
	{
		class LN_API os
		{
		public:
			static std::string openFilaDialog(std::string filter);
		};
	}
}
