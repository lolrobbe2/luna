#pragma once
#include <core/scene/node.h>
#ifndef PT_TO_PX
#define PT_TO_PX(Point) Point * ( 72 / 96 )
#endif // !1
namespace luna 
{
	namespace nodes 
	{
		class Color
		{
		public:
			Color();
			~Color();

		private:

		};

		class canvasItem
		{
			virtual void draw();
		};
	}
}


