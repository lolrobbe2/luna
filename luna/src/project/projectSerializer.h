#pragma once
#include <project/project.h>
namespace luna 
{
	namespace project
	{
		class LN_API projectSerializer
		{
		public:
			static bool serialize(ref<project> project);
			static ref<project> deSerialize(const std::filesystem::path& projectPath);
		};
	}
}


