#pragma once
#include <core/scene/scene.h>
#include <core/platform/platformUtils.h>
namespace luna 
{
	namespace config 
	{
		class LN_API init
		{
		public:
			init(const std::string& filePath = "build/luna.yaml");
			scene* getStartScene();
		private:
			std::string startupScenePath;
		};
	}
}


