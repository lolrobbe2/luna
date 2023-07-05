#pragma once
#include <core/core.h>
namespace luna 
{
	namespace project 
	{
		struct projectConfig
		{
			std::string name = "Untitled";

			std::filesystem::path startScene;

			std::filesystem::path assetDirectory;
			std::filesystem::path scriptModulePath;
			std::filesystem::path projectDirectory;
		};
		class project
		{
		public:
			project(const projectConfig& config);
			projectConfig& getConfig() { return config; }
		private:
			projectConfig config;
			std::filesystem::path projectDirectory;
		};
	}
}


