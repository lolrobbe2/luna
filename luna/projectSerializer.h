#pragma once
#include <project/project.h>
namespace luna 
{
	namespace project
	{
		class projectSerializer
		{
			static bool serialize(ref<project> project);
			static bool deSerialize(ref<project> project);
		};
	}
}


