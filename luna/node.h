#pragma once
#include "lnpch.h"
#include <core/debug/uuid.h>
namespace luna
{
	namespace scene
	{
		class node
		{
		public:
			node();
			~node() = default;
		protected:
			uuid handle;
		};
	}
}


